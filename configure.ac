AC_PREREQ([2.63])
AC_INIT([etherpush], [1], [mende.christoph@gmail.com])
AC_CONFIG_SRCDIR([src/etherpush.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=etherpush
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
		   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

AC_PROG_CC

AC_PATH_PROG(VALAC, valac, valac)
AC_SUBST(VALAC)

AC_MSG_CHECKING([for Win32 platform])
case "`uname`" in
	*MINGW*)
		platform_win32=yes
		VALAFLAGS="-D OS_WIN32"
		;;
	*)
		platform_win32=no
		;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

PKG_PROG_PKG_CONFIG([0.24])
PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.16])

VALAFLAGS+=" --pkg gtk+-2.0 \
	--pkg config \
	--vapidir ../vapi"
AC_SUBST(VALAFLAGS)

AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in])
AC_OUTPUT
