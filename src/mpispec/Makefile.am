
CFLAGS+=-finstrument-functions -rdynamic -Wall

lib_LTLIBRARIES = libmpispec.la

libmpispec_la_SOURCES = mpispec_basic.c \
mpispec.c delay.c mpispec_output_verbose.c mpispec_runner.c \
mpispec_config.c mpispec_private.c mpispec_output.c

pkginclude_HEADERS = mpispec.h mpispec_basic.h \
cspec.h mpispec_config.h mpispec_output.h \
mpispec_output_verbose.h mpispec_private.h
