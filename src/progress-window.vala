/*
 * etherpush - transfer files over network
 *
 * Copyright (C) 2011 Christoph Mende <mende.christoph@gmail.com>
 * All rights reserved. Released under the 2-clause BSD license.
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
