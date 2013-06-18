CFLAGS += -Wall

if DEBUG
CFLAGS += -g
else
CFLAGS += -DNDEBUG -O2
endif

if PROFILE
CFLAGS += -pg
endif

if COVERAGE
CFLAGS += -fprofile-arcs -ftest-coverage
endif

lib_LTLIBRARIES = libcspec.la
libcspec_la_SOURCES = array.c \
cspec_output_header.c \
cspec_output_verbose.c \
cspec_output_xml.c \
cspec_runner.c \
cspec_config.c \
cspec_output_unit.c \
cspec_private.c \
cspec_output.c \
cspec_output_junit_xml.c

pkginclude_HEADERS = mpispec.h array.h cspec.h cspec_config.h cspec_output.h \
cspec_output_header.h cspec_output_junit_xml.h cspec_output_unit.h \
cspec_output_verbose.h cspec_output_xml.h cspec_private.h \
cspec_private_output_junit_xml.h

