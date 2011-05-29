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

void error_dialog(const gchar *fmt, ...)
{
	GtkWidget *dialog;
	gchar *message;
	va_list ap;

	va_start(ap, fmt);
	message = g_strdup_vprintf(fmt, ap);
	va_end(ap);

	dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE, "%s", message);
	gtk_widget_show(dialog);
	g_free(message);

	g_signal_connect_swapped(dialog, "response",
			G_CALLBACK(gtk_widget_destroy), dialog);
}
