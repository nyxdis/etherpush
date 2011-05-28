#include <string.h>
#include <glib.h>
#include <gio/gio.h>

static void connected(GSocketClient*, GAsyncResult*, const gchar*);
static void transfer(GIOStream*, const gchar*);

void send_file(const gchar *host, gchar *filename)
{
	GSocketClient *client;
	client = g_socket_client_new();
	g_socket_client_connect_to_host_async(client, host, 9001, NULL,
			(GAsyncReadyCallback) connected, filename);
	g_object_unref(client);
}

static void connected(GSocketClient *client, GAsyncResult *result,
		const gchar *filename)
{
	GError *error = NULL;
	GSocketConnection *connection;

	connection = g_socket_client_connect_to_host_finish(client, result,
			&error);
	if (connection) {
		g_critical("Connection failed: %s", error->message);
		g_error_free(error);
		return;
	}

	transfer(G_IO_STREAM(connection), filename);
}

static void transfer(GIOStream *stream, const gchar *filename)
{
	GOutputStream *ostream;
	GDataInputStream *dataistream;
	GFile *file;
	GError *error = NULL;
	gchar *header, *content, response;
	gsize length;

	ostream = g_io_stream_get_output_stream(stream);
	dataistream = g_data_input_stream_new(
			g_io_stream_get_input_stream(stream));

	/* load file */
	file = g_file_new_for_path(filename);
	if (!g_file_load_contents(file, NULL, &content, &length, NULL,
				&error)) {
		g_critical("Failed to load file contents: %s", error->message);
		g_error_free(error);
		return;
	}

	/* send header */
	header = g_strdup_printf("\002%s\035%lu\003", filename, length);
	if (g_output_stream_write(ostream, header, strlen(header), NULL,
				&error) < 0) {
		g_free(header);
		g_critical("Failed to write header: %s", error->message);
		g_error_free(error);
		return;
	}
	g_free(header);

	/* check response */
	response = g_data_input_stream_read_byte(dataistream, NULL, &error);
	if (response == 0) {
		g_critical("Failed to read from input stream: %s",
				error->message);
		g_error_free(error);
		return;
	} else if (response == '\025') { /* NAK */
		g_message("[client] File transfer denied");
		return;
	} else if (response != '\006') { /* ACK */
		g_warning("[client] Invalid response");
		return;
	}
	g_message("[client] File transfer accepted");

	/* send file */
	if (g_output_stream_write(ostream, file, length, NULL, &error) < 0) {
		g_critical("Failed to write file: %s", error->message);
		g_error_free(error);
		return;
	}

	/* finish transfer */
	if (g_output_stream_write(ostream, "\005", 1, NULL, &error) < 0) {
		g_critical("Failed to finish byte: %s", error->message);
		g_error_free(error);
		return;
	}

	g_debug("[client] transfer finished");
}
