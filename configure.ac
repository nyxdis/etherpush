#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

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

LOCALEDIR=[${datadir}/locale]
AC_SUBST(ETHERPUSH_LOCALEDIR)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_PROG_PKG_CONFIG([0.24])
PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.16])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in])
AC_OUTPUT
