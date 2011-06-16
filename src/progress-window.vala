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

class ProgressWindow : Window
{
	ProgressBar bar;

	public ProgressWindow(string title)
	{
		set_title(title);
		delete_event.connect(() => { destroy(); return true; });
		set_border_width(10);

		var vbox = new VBox(false, 0);
		add(vbox);

		var hbox = new HBox(false, 0);

		bar = new ProgressBar();
		bar.set_text(title);

		hbox.pack_start(bar, true, false);

		vbox.pack_start(hbox);
		show_all();
	}

	public void set_fraction(double frac)
	{
		if (frac < 1.0)
			bar.set_fraction(frac);
		else
			destroy();
	}
}
