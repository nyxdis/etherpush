/*
 * etherpush - transfer files over network
 *
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 * All rights reserved. Released under the 2-clause BSD license.
 */


using Gtk;

void error_dialog(string message)
{
	var dialog = new MessageDialog(null, 0, MessageType.ERROR, ButtonsType.CLOSE, "%s", message);
	dialog.show();
	dialog.response.connect(() => dialog.destroy());
}
