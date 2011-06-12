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
private const string _etherpush_version = VERSION;

int main(string[] args)
{
	string locale_dir;

#if OS_WIN32
	var installdir = Win32.get_package_installation_directory_of_module(null);
	locale_dir = Path.build_filename(installdir, "share", "locale");
#else
	locale_dir = Path.build_filename(DATADIR, "locale");
#endif
	Intl.bindtextdomain(GETTEXT_PACKAGE, locale_dir);
	Intl.bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	Intl.textdomain(GETTEXT_PACKAGE);

	Gtk.init(ref args);

	start_listener();
	var send_dialog = create_send_dialog();
	create_status_icon(send_dialog);

	Gtk.main();

	send_dialog.destroy();

	return 0;
}
