
# Time-stamp: <2007-11-14 22:32:38 shinya>

# SUBDIRS = mpispec
CFLAGS+=-finstrument-functions -rdynamic -Wall -lcurl -I.

lib_LTLIBRARIES = libmpispec.la

libmpispec_la_SOURCES = \
mpispec/array.c \
mpispec/mpispec.c \
mpispec/mpispec_basic.c \
mpispec/mpispec_output_verbose.c \
mpispec/mpispec_runner.c \
mpispec/mpispec_config.c \
mpispec/mpispec_private.c \
mpispec/mpispec_output.c \
mpispec/mpispec_output_junit_xml.c \
mpispec/mpispec_output_xml.c \
mpispec/mpispec_send.c \
mpispec/mpispec_recv.c \
mpispec/mpispec_isend.c \
mpispec/mpispec_irecv.c \
parson/parson.c \
http-get/http-get.c

pkginclude_HEADERS = \
mpispec/array.h \
mpispec/mpispec.h \
mpispec/mpispec_basic.h \
mpispec/mpispec_config.h \
mpispec/mpispec_consts.h \
mpispec/mpispec_output.h \
mpispec/mpispec_output_junit_xml.h \
mpispec/mpispec_output_verbose.h \
mpispec/mpispec_output_xml.h \
mpispec/mpispec_private.h \
mpispec/mpispec_private_output_junit_xml.h \
mpispec/mpispec_runner.h \
parson/parson.h \
http-get/http-get.h
