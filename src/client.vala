/*
 * etherpush - transfer files over network
 *
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 * All rights reserved. Released under the 2-clause BSD license.
 */


using Config;

// force #include "config.h"
private const string _client_version = VERSION;

async void send_file(string host, string filename)
{
	var client = new SocketClient();
	SocketConnection connection;

	try {
		connection = yield client.connect_to_host_async(host, 9001, null);
	} catch (Error e) {
		error_dialog(_("Connection failed: %s".printf(e.message)));
		return;
	}

	transfer(connection, filename);
}

async void transfer(SocketConnection connection, string filename)
{
	var ostream = connection.get_output_stream();
	var istream = new DataInputStream(connection.get_input_stream());

	int64 filesize;
	try {
		var file = File.new_for_path(filename);
		var file_info = file.query_info(FILE_ATTRIBUTE_STANDARD_SIZE, 0, null);
		filesize = file_info.get_size();
	} catch (Error e) {
		error_dialog(_("Failed to load file: %s".printf(e.message)));
		return;
	}

	var basename = Path.get_basename(filename);
	try {
		yield ostream.write_async(@"\002$basename\035$filesize\003".data);

		var response = istream.read_byte();
		if (response == 25) { /* NAK */
			error_dialog(_("File transfer rejected"));
			return;
		} else if (response != 6) { /* ACK */
			error_dialog(_("Received invalid response"));
			return;
		}

		var file_stream = FileStream.open(filename, "rb");
		var progress_window = new ProgressWindow(_("Sending file"));
		var data = new uint8[4096];
		int64 written = 0;
		while (!file_stream.eof()) {
			var length = file_stream.read(data);
			written += length;
			progress_window.set_fraction((double) written / filesize);
			yield ostream.write_async(data[0:length]);
		}
	} catch (Error err) {
		error_dialog(_("Failed to send file: %s".printf(err.message)));
		return;
	}
}
