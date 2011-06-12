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
using Gtk;

// force #include "config.h"
private const string _incoming_dialog_version = VERSION;

string? incoming_dialog(string in_filename, ssize_t in_length)
{
	string? filename = null;

	var dialog = new MessageDialog(null, 0, MessageType.QUESTION, ButtonsType.OK_CANCEL, "%s", _("Accept file transfer?"));
	dialog.format_secondary_text(_("Filename %s\nSize: %lu"), in_filename, in_length);
	if (dialog.run() == ResponseType.OK) {
		var filechooser = new FileChooserDialog(_("Save file"), null, FileChooserAction.SAVE, Stock.CANCEL, ResponseType.CANCEL, Stock.SAVE, ResponseType.ACCEPT);

		filechooser.set_do_overwrite_confirmation(true);
		filechooser.set_current_folder(Environment.get_home_dir());
		filechooser.set_current_name(in_filename);
		if (filechooser.run() == ResponseType.ACCEPT)
			filename = filechooser.get_filename();
		filechooser.destroy();
	}
	dialog.destroy();

	return filename;
}
