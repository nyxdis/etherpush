AM_CPPFLAGS = -DDATADIR=\""$(datadir)"\"
bin_PROGRAMS = etherpush

etherpush_SOURCES = etherpush.c \
		    send-dialog.c \
		    status-icon.c \
		    client.vala \
		    server.vala \
		    incoming-dialog.vala \
		    error-dialog.c

noinst_HEADERS = send-dialog.h \
		 status-icon.h \
		 client.h \
		 server.h \
		 incoming-dialog.h \
		 error-dialog.h

etherpush_LDADD = $(gtk2_LIBS)

etherpush_CFLAGS = $(gtk2_CFLAGS)

if PLATFORM_WIN32
etherpush_LDFLAGS = -mwindows
endif
