/*
 * etherpush - transfer files over network
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <glib/gi18n.h>

#include "error-dialog.h"

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
	if (!connection) {
		error_dialog(_("Connection failed: %s"), error->message);
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
		error_dialog(_("Failed to load file contents: %s"),
				error->message);
		g_error_free(error);
		return;
	}

	/* send header */
	header = g_strdup_printf("\002%s\035%lu\003", filename, length);
	if (g_output_stream_write(ostream, header, strlen(header), NULL,
				&error) < 0) {
		g_free(header);
		error_dialog(_("Failed to write header: %s"), error->message);
		g_error_free(error);
		return;
	}
	g_free(header);

	/* check response */
	/* TODO this blocks */
	response = g_data_input_stream_read_byte(dataistream, NULL, &error);
	if (response == 0) {
		error_dialog(_("Failed to read from input stream: %s"),
				error->message);
		g_error_free(error);
		return;
	} else if (response == '\025') { /* NAK */
		error_dialog(_("File transfer rejected"));
		return;
	} else if (response != '\006') { /* ACK */
		error_dialog(_("Received invalid response"));
		return;
	}

	/* send file */
	if (g_output_stream_write(ostream, content, length, NULL, &error) < 0) {
		error_dialog(_("Failed to write file: %s"), error->message);
		g_error_free(error);
		return;
	}

	/* finish transfer */
	if (g_output_stream_write(ostream, "\005", 1, NULL, &error) < 0) {
		error_dialog(_("Failed to send finish byte: %s"),
				error->message);
		g_error_free(error);
		return;
	}
}
