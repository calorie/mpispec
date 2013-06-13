
# Time-stamp: <2007-11-14 22:32:56 shinya>

CFLAGS+=-finstrument-functions -rdynamic

lib_LTLIBRARIES = libmpiunit.la libproclog.la libprocut.la

libmpiunit_la_SOURCES = mpiut_basic.c cuerror.c cumem.c testdb.c testrun.c \
util.c mpiunit.c delay.c

libproclog_la_SOURCES = procut_basic.c cuerror.c cumem.c testdb.c testrun.c \
util.c proclog.c delay.c

libprocut_la_SOURCES = procut_basic.c cuerror.c cumem.c testdb.c testrun.c \
util.c procut.c nodelay.c

pkginclude_HEADERS = mpiunit.h mpiut_assertion.h mpiut_basic.h \
cuerror.h cumem.h testdb.h testrun.h util.h mpiut_base.h
