/*
 * etherpush - transfer files over network
 *
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 * All rights reserved. Released under the 2-clause BSD license.
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
