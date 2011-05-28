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

static gboolean listen_incoming(GSocketService*, GSocketService*, GObject*,
		gpointer);

void start_listener(void)
{
	GSocketService *service = g_socket_service_new();
	GInetAddress *addr = g_inet_address_new_any(G_SOCKET_FAMILY_IPV4);
	GSocketAddress *saddr = g_inet_socket_address_new(addr, 9001);
	GError *error = NULL;

	if (!g_socket_listener_add_address(G_SOCKET_LISTENER(service), saddr,
			G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_DEFAULT, NULL,
			NULL, &error)) {
		g_critical("Failed to add listener: %s", error->message);
		g_error_free(error);
		return;
	}

	g_object_unref(addr);
	g_object_unref(saddr);
	g_signal_connect(service, "incoming", G_CALLBACK(listen_incoming), NULL);
	g_socket_service_start(service);
	g_message("[server] started");
}

static gboolean listen_incoming(G_GNUC_UNUSED GSocketService *service,
		GSocketService *connection,
		G_GNUC_UNUSED GObject *source_object,
		G_GNUC_UNUSED gpointer user_data)
{
	GDataInputStream *dataistream;
	GDataOutputStream *dataostream;
	GError *error = NULL;
	GFile *file;
	GOutputStream *ostream;
	gchar byte, *filename, *buffer;
	gint64 length;

	g_debug("[server] file transfer incoming");

	dataistream = g_data_input_stream_new(
			g_io_stream_get_input_stream(G_IO_STREAM(connection)));
	dataostream = g_data_output_stream_new(
			g_io_stream_get_output_stream(G_IO_STREAM(connection)));

	byte = g_data_input_stream_read_byte(dataistream, NULL, &error);
	if (byte == 0) {
		g_critical("Failed to read first byte: %s", error->message);
		g_error_free(error);
		return TRUE;
	}

	if (byte == '\002') { // STX -> filename following
		filename = g_data_input_stream_read_upto(dataistream, "\035",
				-1, NULL, NULL, &error);
		if (filename == NULL) {
			g_critical("Failed to read filename: %s",
					error->message);
			g_error_free(error);
			return;
		}

		filename = g_path_get_basename(filename);

		// consume GS -> size following
		if (g_data_input_stream_read_byte(dataistream, NULL, &error)
				== 0) {
			g_critical("Failed to read: %s", error->message);
			g_error_free(error);
			return;
		}

		buffer = g_data_input_stream_read_upto(dataistream, "\003",
				-1, NULL, NULL, &error);
		if (buffer == NULL) {
			g_critical("Failed to read length: %s",
					error->message);
			g_error_free(error);
			return;
		}
		length = g_ascii_strtoll(buffer, NULL, 10);

		// consume ETX -> file following
		if (g_data_input_stream_read_byte(dataistream, NULL, &error)
				== 0) {
			g_critical("Failed to read: %s", error->message);
			g_error_free(error);
			return;
		}
	} else {
		g_debug("[server] protocol violation");
		return TRUE;
	}

	g_debug("put ACK");
	if (!g_data_output_stream_put_byte(dataostream, '\006', NULL,
				&error)) {
		g_critical("Failed to write response: %s", error->message);
		g_error_free(error);
		return;
	}

	file = g_file_new_for_path(filename);
	g_free(filename);

	// open file for writing
	// TODO: append_to?
	ostream = G_OUTPUT_STREAM(g_file_append_to(file,
				G_FILE_CREATE_REPLACE_DESTINATION, NULL,
				&error));

	buffer = g_data_input_stream_read_upto(dataistream, "\005", -1, NULL,
			NULL, &error);
	if (buffer == NULL) {
		g_critical("Failed to read file: %s", error->message);
		g_error_free(error);
		return;
	}

	if (g_output_stream_write(ostream, buffer, strlen(buffer), NULL,
				&error) < 0) {
		g_critical("Failed to write file: %s", error->message);
		g_error_free(error);
		return;
	}

	// consume EOT
	if (g_data_input_stream_read_byte(dataistream, NULL, &error) == 0) {
		g_critical("Failed to read finish byte: %s", error->message);
		g_error_free(error);
		return;
	}

	g_debug("[server] file transfer done");
	return TRUE;
}
