AM_CPPFLAGS = -DDATADIR=\""$(datadir)"\"
bin_PROGRAMS = etherpush

etherpush_SOURCES = etherpush.vala \
		    client.vala \
		    error-dialog.vala \
		    incoming-dialog.vala \
		    progress-window.vala \
		    send-dialog.vala \
		    server.vala \
		    status-icon.vala

etherpush_LDADD = $(gtk2_LIBS)

etherpush_CFLAGS = $(gtk2_CFLAGS)

if PLATFORM_WIN32
etherpush_LDFLAGS = -mwindows
endif
