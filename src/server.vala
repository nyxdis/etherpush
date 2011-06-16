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


using Config;

// force #include "config.h"
private const string _server_version = VERSION;

void start_listener()
{
	var service = new SocketService();

	try {
		service.add_inet_port(9001, null);
	} catch (Error e) {
		error_dialog(_("Failed to add listener: %s".printf(e.message)));
		return;
	}

	service.incoming.connect((src) => { listen_incoming(src); return true; });
	service.start();
}

async void listen_incoming(SocketConnection conn)
{
	var istream = new DataInputStream(conn.get_input_stream());
	var ostream = new DataOutputStream(conn.get_output_stream());
	string filename, buffer;
	int filesize;
	uchar byte;

	try {
		byte = istream.read_byte();

		if (byte == 2) { // STX -> filename following
			filename = istream.read_upto("\035", -1, null);

			// consume GS -> size following
			istream.read_byte();

			buffer = istream.read_upto("\003", -1, null);
			filesize = int.parse(buffer);

			// consume ETX -> file following
			istream.read_byte();
		} else {
			return;
		}
	} catch (Error e0) {
		error_dialog(_("Failed to read: %s".printf(e0.message)));
		return;
	}

	buffer = incoming_dialog(filename, filesize);

	if (buffer == null) { // cancel transfer
		byte = 24; // NAK
	} else {
		byte = 6; // ACK
	}

	try {
		ostream.put_byte(byte);
	} catch (Error e1) {
		error_dialog(_("Failed to write: %s".printf(e1.message)));
		return;
	}

	if (byte == 24)
		return;

	var file = File.new_for_path(buffer);

	// open file for writing
	FileOutputStream fileostream;
	try {
		fileostream = file.replace(null, false, FileCreateFlags.REPLACE_DESTINATION);
	} catch (Error e2) {
		error_dialog(_("Failed to open file for writing: %s".printf(e2.message)));
		return;
	}

	try {
		var progress_window = new ProgressWindow(_("Receiving file"));
		int read = 0;

		while (read < filesize) {
			var data = new uint8[4096];
			var length = yield istream.read_async(data);
			read += (int) length;
			progress_window.set_fraction((double) read / filesize);

			try {
				fileostream.write(data[0:length]);
			} catch (Error e3) {
				error_dialog(_("Failed to write to file: %s".printf(e3.message)));
				return;
			}
		}
	} catch (Error e4) {
		error_dialog(_("Failed to read: %s".printf(e4.message)));
		return;
	}

	return;
}
