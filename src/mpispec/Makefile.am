CFLAGS+=-finstrument-functions -rdynamic -Wall -lcurl -I../

lib_LTLIBRARIES = libmpispec.la

libmpispec_la_SOURCES = mpispec.c mpispec_basic.c \
mpispec_output_verbose.c mpispec_runner.c \
mpispec_config.c mpispec_private.c mpispec_output.c \
mpispec_output_junit_xml.c mpispec_output_xml.c array.c \
mpispec_isend.c mpispec_irecv.c \
parson/parson.c http-get/http-get.c

pkginclude_HEADERS = array.h mpispec.h mpispec_basic.h \
mpispec_config.h mpispec_consts.h mpispec_output.h \
mpispec_output_junit_xml.h mpispec_output_verbose.h \
mpispec_output_xml.h mpispec_private.h \
mpispec_private_output_junit_xml.h mpispec_runner.h \
parson/parson.h http-get/http-get.h
