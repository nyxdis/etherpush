AM_CPPFLAGS = -DLOCALEDIR=\"$(LOCALEDIR)\"
bin_PROGRAMS = etherpush

etherpush_SOURCES = etherpush.c \
		    send-dialog.c send-dialog.h \
		    status-icon.c status-icon.h \
		    client.c client.h \
		    server.c server.h \
		    incoming-dialog.c incoming-dialog.h

etherpush_LDADD = $(gtk2_LIBS)

etherpush_CFLAGS = $(gtk2_CFLAGS)
