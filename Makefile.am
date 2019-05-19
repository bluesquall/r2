ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = -Wall #-I $(top_srcdir)

lib_LTLIBRARIES = libr2.la

dist_doc_DATA = README.md

# TODO include/install to subdir (e.g., /usr/include/r2/...)
include_HEADERS = \
		r2_epoch.h


libr2_la_SOURCES = libr2.c

libr2_la_CFLAGS = $(AM_CFLAGS)
libr2_la_LDFLAGS = -version-info 0:1 -module -shared -export-dynamic

TESTS = test_r2_epoch

check_PROGRAMS = test_r2_epoch

test_r2_epoch_SOURCES = test/test_r2_epoch.c
test_r2_epoch_CFLAGS = $(AM_CFLAGS)
test_r2_epoch_LDFLAGS = -L@builddir@ -lr2

.PHONY: test
test: check

CLEANFILES = \
	.test*
