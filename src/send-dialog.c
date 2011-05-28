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

#include "config.h"

typedef struct {
	gchar *filename;
	GtkWidget *entry;
} transfer_info_t;

static void file_chosen(GtkFileChooser*, transfer_info_t*);
static void start_transfer(GtkWidget*, transfer_info_t*);

GtkWidget* create_send_dialog(void)
{
	GtkWidget *window, *button, *vbox, *hbox;
	transfer_info_t *transfer_info = g_new(transfer_info_t, 1);

	transfer_info->filename = NULL;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window), PACKAGE_NAME);
	g_signal_connect_swapped(window, "delete-event",
			G_CALLBACK(gtk_widget_hide), window);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	/* vbox containing hboxes */
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	/* ip entry box */
	hbox = gtk_hbox_new(FALSE, 0);

	transfer_info->entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), transfer_info->entry, TRUE, TRUE, 0);
	gtk_widget_show(transfer_info->entry);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
	gtk_widget_show(hbox);


	/* file/send buttons */
	hbox = gtk_hbox_new(FALSE, 0);

	/* file chooser button */
	button = gtk_file_chooser_button_new(_("File to send"),
			GTK_FILE_CHOOSER_ACTION_OPEN);
	g_signal_connect(button, "file-set", G_CALLBACK(file_chosen),
			transfer_info);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(button),
			g_get_home_dir());
	gtk_file_chooser_button_set_width_chars(
			GTK_FILE_CHOOSER_BUTTON(button), 15);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	/* send button */
	button = gtk_button_new_with_label(_("Send"));
	g_signal_connect(button, "clicked", G_CALLBACK(start_transfer),
			transfer_info);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
	gtk_widget_show(hbox);

	/* hide button */
	hbox = gtk_hbox_new(FALSE, 0);

	button = gtk_button_new_with_label(_("Hide"));
	g_signal_connect_swapped(button, "clicked",
			G_CALLBACK(gtk_widget_hide), window);
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
	gtk_widget_show(hbox);

	gtk_widget_show(vbox);

	return window;
}

static void file_chosen(GtkFileChooser *widget, transfer_info_t *transfer_info)
{
	g_free(transfer_info->filename);
	transfer_info->filename = gtk_file_chooser_get_filename(widget);
}

static void start_transfer(G_GNUC_UNUSED GtkWidget *widget, transfer_info_t *transfer_info)
{
	const gchar *destination;
	destination = gtk_entry_get_text(GTK_ENTRY(transfer_info->entry));
	if (transfer_info->filename == NULL || strlen(destination) == 0)
		return;
	send_file(destination, transfer_info->filename);
}
