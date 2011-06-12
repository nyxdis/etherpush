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

// force #include "config.h"
private const string _status_icon_version = VERSION;

StatusIcon create_status_icon(Window main_window)
{
	/* build popup menu */
	var menu = new Menu();
	var item = new MenuItem.with_label(_("Show"));
	menu.append(item);
	item.activate.connect(() => { main_window.show(); });

	item = new MenuItem.with_label(_("Quit"));
	menu.append(item);
	item.activate.connect(Gtk.main_quit);

	var status_icon = new StatusIcon.from_stock(Stock.CONNECT);
	status_icon.set_visible(true);
	status_icon.set_tooltip_text(PACKAGE_STRING);
	status_icon.activate.connect(() => { icon_activate(main_window); });
	status_icon.popup_menu.connect((button, activate_time) => { icon_popup(status_icon, button, activate_time, menu); });

	return status_icon;
}

void icon_activate(Window main_window)
{
	main_window.set_visible(!main_window.get_visible());
}

void icon_popup(StatusIcon status_icon, uint button, uint activate_time, Menu menu)
{
	menu.show_all();
	menu.popup(null, null, status_icon.position_menu, button, activate_time);
}
