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

static void icon_activate(GtkStatusIcon*, gpointer);
static void icon_popup(GtkStatusIcon*, guint, guint, gpointer);

GtkStatusIcon* create_status_icon(GtkWidget *main_window)
{
	GtkWidget *menu, *item;
	GtkStatusIcon *status_icon;

	/* build popup menu */
	menu = gtk_menu_new();
	item = gtk_menu_item_new_with_label(_("Show"));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	g_signal_connect_swapped(item, "activate", G_CALLBACK(gtk_widget_show),
			main_window);

	item = gtk_menu_item_new_with_label(_("Quit"));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	g_signal_connect(item, "activate", G_CALLBACK(gtk_main_quit), NULL);

	status_icon = gtk_status_icon_new_from_stock(GTK_STOCK_CONNECT);
	gtk_status_icon_set_visible(status_icon, TRUE);
	gtk_status_icon_set_tooltip_text(status_icon, PACKAGE_STRING);
	g_signal_connect(status_icon, "activate", G_CALLBACK(icon_activate),
			main_window);
	g_signal_connect(status_icon, "popup-menu", G_CALLBACK(icon_popup),
			menu);

	return status_icon;
}

static void icon_activate(G_GNUC_UNUSED GtkStatusIcon *status_icon,
		gpointer user_data)
{
	gtk_widget_set_visible(GTK_WIDGET(user_data),
			!gtk_widget_get_visible(GTK_WIDGET(user_data)));
}

static void icon_popup(GtkStatusIcon *status_icon, guint button,
		guint activate_time, gpointer user_data)
{
	gtk_widget_show_all(GTK_WIDGET(user_data));
	gtk_menu_popup(GTK_MENU(user_data), NULL, NULL,
			gtk_status_icon_position_menu, status_icon, button,
			activate_time);
}
