CC=mpicc
CFLAGS+=-rdynamic -Wall -Ihash -Impispec

lib_LTLIBRARIES = libmpispec.la

libmpispec_la_SOURCES = \
    hash/hash.c \
    mpispec/array.c \
    mpispec/mpispec_basic.c \
    mpispec/mpispec_comm_world.c \
    mpispec/mpispec_hash.c \
    mpispec/mpispec_irecv.c \
    mpispec/mpispec_isend.c \
    mpispec/mpispec_output.c \
    mpispec/mpispec_output_junit_xml.c \
    mpispec/mpispec_output_verbose.c \
    mpispec/mpispec_output_xml.c \
    mpispec/mpispec_private.c \
    mpispec/mpispec_runner.c \
    mpispec/mpispec_stub.c \
    mpispec/mpispec_summary.c \
    mpispec/mpispec_util.c \
    mpispec/mpispec_wait.c

pkginclude_HEADERS = \
    hash/hash.h \
    hash/khash.h \
    mpispec/array.h \
    mpispec/mpispec.h \
    mpispec/mpispec_basic.h \
    mpispec/mpispec_comm_world.h \
    mpispec/mpispec_hash.h \
    mpispec/mpispec_output.h \
    mpispec/mpispec_output_junit_xml.h \
    mpispec/mpispec_output_verbose.h \
    mpispec/mpispec_output_xml.h \
    mpispec/mpispec_private.h \
    mpispec/mpispec_private_output_junit_xml.h \
    mpispec/mpispec_runner.h \
    mpispec/mpispec_stub.h \
    mpispec/mpispec_summary.h \
    mpispec/mpispec_util.h
