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


#include <gtk/gtk.h>
#include <glib/gi18n.h>

gchar* incoming_dialog(gchar *in_filename, gsize in_length)
{
	GtkWidget *dialog, *filechooser;
	gchar *filename = NULL;

	dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_OK_CANCEL, _("Accept file transfer?"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
			"Filename: %s\nSize: %lu", in_filename, in_length);
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		filechooser = gtk_file_chooser_dialog_new("Save file", NULL,
				GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
				GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE,
				GTK_RESPONSE_ACCEPT, NULL);
		gtk_file_chooser_set_do_overwrite_confirmation(
				GTK_FILE_CHOOSER(filechooser), TRUE);
		gtk_file_chooser_set_current_folder(
				GTK_FILE_CHOOSER(filechooser),
				g_get_home_dir());
		gtk_file_chooser_set_current_name(
				GTK_FILE_CHOOSER(filechooser), in_filename);
		if (gtk_dialog_run(GTK_DIALOG(filechooser))
				== GTK_RESPONSE_ACCEPT) {
			filename = gtk_file_chooser_get_filename(
					GTK_FILE_CHOOSER(filechooser));
		}
		gtk_widget_destroy(filechooser);
	}
	gtk_widget_destroy(dialog);

	return filename;
}
