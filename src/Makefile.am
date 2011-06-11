AM_CPPFLAGS = -DDATADIR=\""$(datadir)"\"
bin_PROGRAMS = etherpush

etherpush_SOURCES = etherpush.c \
		    send-dialog.c \
		    status-icon.c \
		    client.c \
		    server.c \
		    incoming-dialog.vala \
		    error-dialog.c

etherpush_LDADD = $(gtk2_LIBS)

etherpush_CFLAGS = $(gtk2_CFLAGS)

if PLATFORM_WIN32
etherpush_LDFLAGS = -mwindows
endif
