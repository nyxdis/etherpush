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


using Gtk;
using Config;

struct TransferInfo {
	string? filename;
	Entry entry;
}

Window create_send_dialog()
{
	var transfer_info = TransferInfo();
	transfer_info.filename = null;

	var window = new Window();

	window.set_title(PACKAGE_NAME);
	window.delete_event.connect(() => { window.hide(); return true; });
	window.set_border_width(10);

	/* vbox containing hboxes */
	var vbox = new VBox(false, 0);
	window.add(vbox);

	/* ip entry box */
	var hbox = new HBox(false, 0);

	transfer_info.entry = new Entry();
	hbox.pack_start(transfer_info.entry);
	transfer_info.entry.show();

	vbox.pack_start(hbox);
	hbox.show();


	/* file/send buttons */
	hbox = new HBox(false, 0);

	/* file chooser button */
	var file_chooser = new FileChooserButton(_("File to send"), FileChooserAction.OPEN);
	file_chooser.file_set.connect(() => { file_chosen(file_chooser, transfer_info); });
	file_chooser.set_current_folder(Environment.get_home_dir());
	file_chooser.set_width_chars(15);
	hbox.pack_start(file_chooser);
	file_chooser.show();

	/* send button */
	var button = new Button.with_label(_("Send"));
	button.clicked.connect(() => { start_transfer(transfer_info); });
	hbox.pack_start(button);
	button.show();

	vbox.pack_start(hbox);
	hbox.show();

	/* hide button */
	hbox = new HBox(false, 0);

	button = new Button.with_label(_("Hide"));
	button.clicked.connect(() => { window.hide(); });
	hbox.pack_start(button);
	button.show();

	vbox.pack_start(hbox);
	hbox.show();

	vbox.show();

	return window;
}

void file_chosen(FileChooserButton file_chooser, TransferInfo transfer_info)
{
	transfer_info.filename = file_chooser.get_filename();
}

void start_transfer(TransferInfo transfer_info)
{
	string destination = transfer_info.entry.get_text();
	if (transfer_info.filename == null || destination == null)
		return;
	send_file(destination, transfer_info.filename);
}
