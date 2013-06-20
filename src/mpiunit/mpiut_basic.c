
/* Time-stamp: <2007-11-24 23:04:56 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

/*
 *  Original:
 *
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Implementation for basic test runner interface.
 *
 *  Created By  : Jerry St.Clair  (11-Aug-2004)
 *  Comment     : Initial implementation of basic test runner interface
 *  EMail       : jds2@users.sourceforge.net
 *
 *  Modified    : 8-Jan-2005 (JDS)
 *  Comment     : Fixed reporting bug (bug report cunit-Bugs-1093861).
 *  Email       : jds2@users.sourceforge.net
 *
 *  Modified    : 30-Apr-2005 (JDS)
 *  Comment     : Added notification of suite cleanup failure.
 *  Email       : jds2@users.sourceforge.net
 */

/** @file
 * Basic interface with output to stdout.
 */
/** @addtogroup Basic
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <mpi.h>

#include "mpiut_base.h"
#include "testdb.h"
#include "util.h"
#include "testrun.h"
#include "mpiut_basic.h"
#include "../cspec/cspec_output.h"
FILE *__mpiut_result_file__;
/* MPISpecRunSummary mpi_run_summary; */

static CU_pSuite f_pRunningSuite = NULL;
static CU_BasicRunMode f_run_mode = CU_BRM_NORMAL;

static CU_ErrorCode basic_initialize(void);
static CU_ErrorCode basic_run_all_tests(CU_pTestRegistry pRegistry);
static CU_ErrorCode basic_run_suite(CU_pSuite pSuite);
static CU_ErrorCode basic_run_single_test(CU_pSuite pSuite, CU_pTest pTest);

static void
basic_test_start_message_handler(const CU_pTest pTest,
                                 const CU_pSuite pSuite);

static void
basic_test_complete_message_handler(const CU_pTest pTest,
                                    const CU_pSuite pSuite,
                                    const CU_pFailureRecord pFailureList);

static void
basic_all_tests_complete_message_handler(const CU_pFailureRecord pFailure);

static void
basic_suite_init_failure_message_handler(const CU_pSuite pSuite);

static void
basic_suite_cleanup_failure_message_handler(const CU_pSuite pSuite);

CU_ErrorCode
CU_basic_run_tests(void)
{
  CU_ErrorCode error;

  if (NULL == CU_get_registry()) {
    if (CU_BRM_SILENT != f_run_mode)
      fprintf(stderr, "\n\nFATAL ERROR - Test registry is not initialized.\n");
    error = CUE_NOREGISTRY;
  }
  else if (CUE_SUCCESS == (error = basic_initialize()))
    error = basic_run_all_tests(NULL);

  return error;
}

CU_ErrorCode
CU_basic_run_suite(CU_pSuite pSuite)
{
  CU_ErrorCode error;

  if (NULL != pSuite)
    error = CUE_NOSUITE;
  else if (CUE_SUCCESS == (error = basic_initialize()))
    error = basic_run_suite(pSuite);

  return error;
}

CU_ErrorCode
CU_basic_run_test(CU_pSuite pSuite, CU_pTest pTest)
{
  CU_ErrorCode error;

  if (NULL != pSuite)
    error = CUE_NOSUITE;
  else if (NULL != pTest)
    error = CUE_NOTEST;
  else if (CUE_SUCCESS == (error = basic_initialize())) {
    error = basic_run_single_test(pSuite, pTest);
  }

  return error;
}

void
CU_basic_set_mode(CU_BasicRunMode mode)
{
  int myrank;
  char result_filename[32];

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  sprintf(result_filename, "rank%d.result", myrank);
  if(NULL == (__mpiut_result_file__ = fopen(result_filename, "a"))) {
    fprintf(stderr, "Can't open result files");
    exit(-1);
  };
  fprintf(__mpiut_result_file__, "\nrank  %d:", myrank);

  f_run_mode = mode;
}

CU_BasicRunMode
CU_basic_get_mode(void)
{
  return f_run_mode;
}

void
CU_basic_show_failures(CU_pFailureRecord pFailure)
{
  int i;

  for (i = 1 ; (NULL != pFailure) ; pFailure = pFailure->pNext, i++) {
/*     fprintf(__mpiut_result_file__, "\n  %d. %s:%u  - %s", i, */
/*             (NULL != pFailure->strFileName) ? pFailure->strFileName : "", */
/*             pFailure->uiLineNumber, */
/*             (NULL != pFailure->strCondition) ? pFailure->strCondition : ""); */
    fprintf(stdout, "\n  %d. %s:%u  - %s\n", i,
            (NULL != pFailure->strFileName) ? pFailure->strFileName : "",
            pFailure->uiLineNumber,
            (NULL != pFailure->strCondition) ? pFailure->strCondition : "");
  }
}

void
CU_basic_exit()
{
  fclose(__mpiut_result_file__);
}

static CU_ErrorCode
basic_initialize(void)
{
  setvbuf(__mpiut_result_file__, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  CU_set_error(CUE_SUCCESS);

  if (CU_BRM_SILENT != f_run_mode){
    /* puts(""); */
  }

  CU_set_test_start_handler(basic_test_start_message_handler);
  CU_set_test_complete_handler(basic_test_complete_message_handler);
  CU_set_all_test_complete_handler(basic_all_tests_complete_message_handler);
  CU_set_suite_init_failure_handler(basic_suite_init_failure_message_handler);
  CU_set_suite_cleanup_failure_handler(basic_suite_cleanup_failure_message_handler);

  return CU_get_error();
}

static CU_ErrorCode
basic_run_all_tests(CU_pTestRegistry pRegistry)
{
  CU_pTestRegistry pOldRegistry = NULL;
  CU_ErrorCode result;

  f_pRunningSuite = NULL;

  if (NULL != pRegistry)
    pOldRegistry = CU_set_registry(pRegistry);
  result = CU_run_all_tests();
  if (NULL != pRegistry)
    CU_set_registry(pOldRegistry);
  return result;
}

static CU_ErrorCode
basic_run_suite(CU_pSuite pSuite)
{
  f_pRunningSuite = NULL;
  return CU_run_suite(pSuite);
}

static CU_ErrorCode
basic_run_single_test(CU_pSuite pSuite, CU_pTest pTest)
{
  f_pRunningSuite = NULL;
  return CU_run_test(pSuite, pTest);
}

static void
basic_test_start_message_handler(const CU_pTest pTest,
                                 const CU_pSuite pSuite)
{
  assert(NULL != pSuite);
  assert(NULL != pTest);

  if (CU_BRM_VERBOSE == f_run_mode) {
    if ((NULL == f_pRunningSuite) || (f_pRunningSuite != pSuite)) {
      fprintf(__mpiut_result_file__, "\n%s:", (NULL != pSuite->pName) ? pSuite->pName : "");
      /* fprintf(__mpiut_result_file__, "\n  Test: %s ... ", (NULL != pTest->pName) ? pTest->pName : ""); */
      f_pRunningSuite = pSuite;
    }
    else {
      fprintf(__mpiut_result_file__, "\n  Test: %s ... ", (NULL != pTest->pName) ? pTest->pName : "");
    }
  }
}

static void
basic_test_complete_message_handler(const CU_pTest pTest,
                                    const CU_pSuite pSuite,
                                    const CU_pFailureRecord pFailureList)
{
  CU_pFailureRecord pFailure = pFailureList;
  int i;

  assert(NULL != pSuite);
  assert(NULL != pTest);

  if (NULL == pFailure) {
    if (CU_BRM_VERBOSE == f_run_mode) {
      /* fprintf(__mpiut_result_file__, "\033[1;32mpassed\033[0m"); */
    }
  }
  else {
    switch (f_run_mode) {
      case CU_BRM_VERBOSE:
        /* fprintf(__mpiut_result_file__, "\033[1;31mFAILED\033[0m"); */
        break;
      case CU_BRM_NORMAL:
        fprintf(__mpiut_result_file__, "\nSuite %s, Test %s had failures:",
                        (NULL != pSuite->pName) ? pSuite->pName : "",
                        (NULL != pTest->pName) ? pTest->pName : "");
        break;
      default:
        break;
    }
    if (CU_BRM_SILENT != f_run_mode) {
      for (i = 1 ; (NULL != pFailure) ; pFailure = pFailure->pNext, i++) {
        fprintf(__mpiut_result_file__, "\n    %d. %s:%u  - %s", i,
            (NULL != pFailure->strFileName) ? pFailure->strFileName : "",
            pFailure->uiLineNumber,
            (NULL != pFailure->strCondition) ? pFailure->strCondition : "");
      }
    }
  }
}

static void
basic_all_tests_complete_message_handler(const CU_pFailureRecord pFailure)
{
  CU_pRunSummary pRunSummary = CU_get_run_summary();
  CU_pTestRegistry pRegistry = CU_get_registry();

  MS_pRunSummary summary = get_mpi_run_summary();

  CU_UNREFERENCED_PARAMETER(pFailure);

  assert(NULL != pRunSummary);
  assert(NULL != pRegistry);

  if (CU_BRM_SILENT != f_run_mode){
    /* fprintf(__mpiut_result_file__,"\n--Run Summary: Type      Total  Passed  Failed" */
    /*                  "\n               tests  %8u%8u%8u" */
    /*                  "\n               asserts%8u%8u%8u\n", */
    /*                 pRegistry->uiNumberOfTests, */
    /*                 pRunSummary->nTestsRun - pRunSummary->nTestsFailed, */
    /*                 pRunSummary->nTestsFailed, */
    /*                 pRunSummary->nAsserts, */
    /*                 pRunSummary->nAsserts - pRunSummary->nAssertsFailed, */
    /*                 pRunSummary->nAssertsFailed); */
    fprintf(__mpiut_result_file__,"\n--Run Summary: Type      Total  Passed  Failed"
                     "\n               tests  %8u%8u%8u\n",
                    summary->Total,
                    summary->Passed,
                    summary->Total - summary->Passed);
  }
}

static void
basic_suite_init_failure_message_handler(const CU_pSuite pSuite)
{
  assert(NULL != pSuite);

  if (CU_BRM_SILENT != f_run_mode)
    fprintf(__mpiut_result_file__,
            "\nWARNING - Suite initialization failed for %s.",
            (NULL != pSuite->pName) ? pSuite->pName : "");
}

static void
basic_suite_cleanup_failure_message_handler(const CU_pSuite pSuite)
{
  assert(NULL != pSuite);

  if (CU_BRM_SILENT != f_run_mode)
    fprintf(__mpiut_result_file__,
            "\nWARNING - Suite cleanup failed for %s.",
            (NULL != pSuite->pName) ? pSuite->pName : "");
}

void
mpispec_run_summary(void)
{
  MS_pRunSummary summary = get_mpi_run_summary();

  fprintf(__mpiut_result_file__,"\n--Run Summary: Type      Total  Passed  Failed"
      "\n               tests  %8u%8u%8u\n",
      summary->Total,
      summary->Passed,
      summary->Total - summary->Passed);
}
