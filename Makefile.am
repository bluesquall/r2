ACLOCAL_AMFLAGS = -I .build-aux/m4
EXTRA_DIST = .build-aux/git-version-gen

dist_doc_DATA = README.md
pkgconfig_DATA = r2.pc
pkginclude_HEADERS = \
		r2_epoch.h \
		r2_timerfd.h

TESTS = test-r2_epoch

check_PROGRAMS = $(TESTS)

test_r2_epoch_SOURCES = test/test_r2_epoch.c
test_r2_epoch_CFLAGS = $(AM_CFLAGS)
