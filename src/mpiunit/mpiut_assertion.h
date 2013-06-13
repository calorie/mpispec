
/* Time-stamp: <2007-11-14 22:36:40 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#ifndef MPIUT_ASSERTION_H
#define MPIUT_ASSERTION_H

#include "mpi.h"

#define MPIUT_PASS(msg) \
CU_assertImplementation( \
  CU_TRUE, \
  __LINE__, \
  ("MPIUT_PASS(" #msg ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_FAIL(msg) \
CU_assertImplementation( \
  CU_FALSE, \
  __LINE__, \
  ("MPIUT_FAIL(" #msg ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_TRUE(value) \
CU_assertImplementation( \
  (value), \
  __LINE__, \
  ("MPIUT_ASSERT_TRUE(" #value ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_FALSE(value)   \
CU_assertImplementation( \
  !(value), \
  __LINE__, \
  ("MPIUT_ASSERT_FALSE(" #value ")"),\
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_INT_EQUAL(actual, expect) \
CU_assertImplementation( \
  ((actual) == (expect)), \
  __LINE__, \
  ("MPIUT_ASSERT_INT_EQUAL(" #actual "," #expect ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_INT_NOT_EQUAL(actual, expect)   \
CU_assertImplementation( \
  ((actual) != (expect)), \
  __LINE__, \
  ("MPIUT_ASSERT_INT_NOT_EQUAL(" #actual "," #expect ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_STRING_EQUAL(actual, expect)  \
CU_assertImplementation( \
  !(strcmp((const char*)(actual), (const char*)(expect))), \
  __LINE__, \
  ("MPIUT_ASSERT_STRING_EQUAL(" #actual "," #expect ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_STRING_NOT_EQUAL(actual, expect)  \
CU_assertImplementation( \
  (strcmp((const char*)(actual), (const char*)(expect))), \
  __LINE__, \
  ("MPIUT_ASSERT_STRING_NOT_EQUAL(" #actual ","  #expect ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_DOUBLE_EQUAL(actual, expect, granularity) \
CU_assertImplementation( \
  ((fabs((double)(actual) - (expect)) <= fabs((double)(granularity)))), \
  __LINE__, \
  ("MPIUT_ASSERT_DOUBLE_EQUAL(" #actual ","  #expect "," \
                              #granularity ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#define MPIUT_ASSERT_DOUBLE_NOT_EQUAL(actual, expect, granularity) \
CU_assertImplementation( \
  ((fabs((double)(actual) - (expect)) > fabs((double)(granularity)))), \
  __LINE__, \
  ("MPIUT_ASSERT_DOUBLE_NOT_EQUAL(" #actual ","  #expect "," \
                                  #granularity ")"), \
  __FILE__, \
  "", \
  CU_FALSE)

#endif
