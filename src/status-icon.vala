/*
 * etherpush - transfer files over network
 *
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 * All rights reserved. Released under the 2-clause BSD license.
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
