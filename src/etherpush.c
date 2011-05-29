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

#include "send-dialog.h"
#include "status-icon.h"
#include "server.h"

int main(int argc, char *argv[])
{
	GtkWidget *send_dialog;
	GtkStatusIcon *status_icon;
	GError *error = NULL;
	gchar *locale_dir;

#ifdef G_OS_WIN32
	gchar *installdir;
	installdir = g_win32_get_package_installation_directory_of_module(NULL);
	locale_dir = g_build_filename(installdir, "share", "locale", NULL);
	g_free(installdir);
#else
	locale_dir = g_build_filename(DATADIR, "locale", NULL);
#endif
	bindtextdomain(GETTEXT_PACKAGE, locale_dir);
	g_free(locale_dir);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	if (!gtk_init_with_args(&argc, &argv, "", NULL, NULL, &error)) {
		g_printerr(_("Error: %s\n"), error->message);
		return 1;
	}

	start_listener();
	send_dialog = create_send_dialog();
	status_icon = create_status_icon(send_dialog);

	gtk_main();

	gtk_widget_destroy(send_dialog);
	g_object_unref(status_icon);

	return 0;
}
