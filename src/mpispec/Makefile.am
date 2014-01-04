
CFLAGS+=-finstrument-functions -rdynamic -Wall

lib_LTLIBRARIES = libmpispec.la

libmpispec_la_SOURCES = mpispec_basic.c \
mpispec.c delay.c mpispec_output_verbose.c mpispec_runner.c \
mpispec_config.c mpispec_private.c mpispec_output.c mpi_stub.c \
mpispec_output_junit_xml.c mpispec_output_xml.c array.c

pkginclude_HEADERS = mpispec.h mpispec_basic.h \
mpi_stub.h mpispec_config.h mpispec_output.h \
mpispec_output_verbose.h mpispec_private.h \
mpispec_output_junit_xml.h mpispec_private_output_junit_xml.h \
mpispec_consts.h array.h mpispec_output_xml.h

