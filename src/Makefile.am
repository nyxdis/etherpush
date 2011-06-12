AM_CPPFLAGS = -DDATADIR=\""$(datadir)"\"
bin_PROGRAMS = etherpush

etherpush_SOURCES = etherpush.vala \
		    send-dialog.vala \
		    status-icon.vala \
		    client.vala \
		    server.vala \
		    incoming-dialog.vala \
		    error-dialog.vala

etherpush_LDADD = $(gtk2_LIBS)

etherpush_CFLAGS = $(gtk2_CFLAGS)

if PLATFORM_WIN32
etherpush_LDFLAGS = -mwindows
endif
