AC_INIT([r2],
	m4_esyscmd([.build-aux/git-version-gen .tarball-version]),
	[https://bitbucket.org/bluesquall/r2/issues],
	[],
	[https://bitbucket.org/bluesquall/r2],
)
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIRS([.build-aux/m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects dist-xz -Wall -Wno-portability foreign])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

AM_PROG_AR
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_PROG_CC_STDC

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([r2.pc:r2.pc.in])


AM_PROG_AR
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_PROG_CC_STDC
AC_OUTPUT

LT_INIT([dlopen])
