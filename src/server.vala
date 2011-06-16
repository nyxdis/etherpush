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

	service.incoming.connect(listen_incoming);
	service.start();
}

bool listen_incoming(SocketConnection conn)
{
	var istream = new DataInputStream(conn.get_input_stream());
	var ostream = new DataOutputStream(conn.get_output_stream());
	string filename, buffer;
	int length;
	uchar byte;

	try {
		byte = istream.read_byte();

		if (byte == 2) { // STX -> filename following
			filename = istream.read_upto("\035", -1, null);

			// consume GS -> size following
			istream.read_byte();

			buffer = istream.read_upto("\003", -1, null);
			length = int.parse(buffer);

			// consume ETX -> file following
			istream.read_byte();
		} else {
			return true;
		}
	} catch (Error e) {
		error_dialog(_("Failed to read: %s".printf(e.message)));
		return true;
	}

	buffer = incoming_dialog(filename, length);

	if (buffer == null) { // cancel transfer
		byte = 24; // NAK
	} else {
		byte = 6; // ACK
	}

	try {
		ostream.put_byte(byte);
	} catch (Error e) {
		error_dialog(_("Failed to write: %s".printf(e.message)));
		return true;
	}

	if (byte == 24)
		return true;

	var file = File.new_for_path(buffer);

	// open file for writing
	FileOutputStream fileostream;
	try {
		fileostream = file.create(FileCreateFlags.REPLACE_DESTINATION);
	} catch (Error e) {
		error_dialog(_("Failed to open file for writing: %s".printf(e.message)));
		return true;
	}

	try {
		buffer = istream.read_upto("\005", -1, null);
	} catch (Error e) {
		error_dialog(_("Failed to read: %s".printf(e.message)));
		return true;
	}

	try {
		fileostream.write(buffer.data);
	} catch (Error e) {
		error_dialog(_("Failed to write to file: %s".printf(e.message)));
		return true;
	}

	// consume EOT
	try {
		istream.read_byte();
	} catch (Error e) {
		error_dialog(_("Failed to read: %s".printf(e.message)));
		return true;
	}

	return true;
}
