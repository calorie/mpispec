#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mpispec], [1.1.0], [s2g4t1n2@gmail.com])
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([subdir-objects])

AC_ARG_ENABLE(debug,
[  --enable-debug          turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

AC_ARG_ENABLE(profile,
[  --enable-profile        turn on profiling],
[case "${enableval}" in
  yes) profile=true ;;
  no)  profile=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-profile) ;;
esac],[profile=false])
AM_CONDITIONAL(PROFILE, test x$profile = xtrue)

AC_ARG_ENABLE(coverage,
[  --enable-coverage       turn on coverage feature],
[case "${enableval}" in
  yes) coverage=true ;;
  no)  coverage=false ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-coverage) ;;
esac],[coverage=false])
AM_CONDITIONAL(COVERAGE, test x$coverage = xtrue)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
#AC_PROG_LIBTOOL

# Checks for libraries.
LT_INIT

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h memory.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memset strerror strdup])

AC_CONFIG_FILES([Makefile
                 script/Makefile
                 script/mpispec
                 share/Makefile
                 src/Makefile])
AC_OUTPUT
