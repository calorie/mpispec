
/* Time-stamp: <2007-11-14 22:37:25 shinya> */

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
 *  Copyright (C) 2001            Anil Kumar
 *  Copyright (C) 2004,2005,2006  Anil Kumar, Jerry St.Clair
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
 *  Implementation of Registry/TestGroup/Testcase management Routines.
 *
 *  Aug 2001        Initial implementation (AK)
 *
 *  09/Aug/2001     Added startup initialize/cleanup registry functions. (AK)
 *
 *  29/Aug/2001     Added Test and Group Add functions. (AK)
 *
 *  02/Oct/2001     Added Proper Error codes and Messages on the failure conditions. (AK)
 *
 *  13/Oct/2001     Added Code to Check for the Duplicate Group name and test name. (AK)
 *
 *  15-Jul-2004     Added doxygen comments, new interface, added assertions to
 *                  internal functions, moved error handling code to CUError.c,
 *                  added assertions to make sure no modification of registry
 *                  during a run, bug fixes, changed CU_set_registry() so that it
 *                  doesn't require cleaning the existing registry. (JDS)
 */

/** @file
 *  Management functions for tests, suites, and the test registry (implementation).
 */
/** @addtogroup Framework
 @{
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "mpiut_base.h"
#include "cumem.h"
#include "testdb.h"
#include "testrun.h"
#include "util.h"

static CU_pTestRegistry f_pTestRegistry = NULL;

static void
cleanup_test_registry(CU_pTestRegistry pRegistry);

static CU_pSuite
create_suite(const char* strName, CU_InitializeFunc pInit,
             CU_CleanupFunc pClean);

static void
cleanup_suite(CU_pSuite pSuite);

static void
insert_suite(CU_pTestRegistry pRegistry, CU_pSuite pSuite);

static CU_pTest
create_test(const char* strName, CU_TestFunc pTestFunc);

static void
cleanup_test(CU_pTest pTest);

static void
insert_test(CU_pSuite pSuite, CU_pTest pTest);

static CU_BOOL
suite_exists(CU_pTestRegistry pRegistry, const char* szSuiteName);

static CU_BOOL
test_exists(CU_pSuite pSuite, const char* szTestName);

CU_ErrorCode
CU_initialize_registry(void)
{
  CU_ErrorCode result;

  assert(CU_FALSE == CU_is_test_running());

  CU_set_error(result = CUE_SUCCESS);

  if (NULL != f_pTestRegistry) {
    CU_cleanup_registry();
  }

  f_pTestRegistry = CU_create_new_registry();
  if (NULL == f_pTestRegistry) {
    CU_set_error(result = CUE_NOMEMORY);
  }

  return result;
}

CU_BOOL
CU_registry_initialized(void)
{
  return (NULL == f_pTestRegistry) ? CU_FALSE : CU_TRUE;
}

void
CU_cleanup_registry(void)
{
  assert(CU_FALSE == CU_is_test_running());

  CU_set_error(CUE_SUCCESS);
  CU_destroy_existing_registry(&f_pTestRegistry);
  CU_clear_previous_results();
  CU_CREATE_MEMORY_REPORT(NULL);
}

CU_pTestRegistry
CU_get_registry(void)
{
  return f_pTestRegistry;
}

CU_pTestRegistry
CU_set_registry(CU_pTestRegistry pRegistry)
{
  CU_pTestRegistry pOldRegistry = f_pTestRegistry;

  assert(CU_FALSE == CU_is_test_running());

  CU_set_error(CUE_SUCCESS);
  f_pTestRegistry = pRegistry;
  return pOldRegistry;
}

CU_pSuite
CU_add_suite(const char* strName, CU_InitializeFunc pInit,
             CU_CleanupFunc pClean)
{
  CU_pSuite pRetValue = NULL;
  CU_ErrorCode error = CUE_SUCCESS;

  assert(CU_FALSE == CU_is_test_running());

  if (NULL == f_pTestRegistry) {
    error = CUE_NOREGISTRY;
  }
  else if (NULL == strName) {
    error = CUE_NO_SUITENAME;
  }
  else if (CU_TRUE == suite_exists(f_pTestRegistry, strName)) {
    error = CUE_DUP_SUITE;
  }
  else {
    pRetValue = create_suite(strName, pInit, pClean);
    if (NULL == pRetValue) {
      error = CUE_NOMEMORY;
    }
    else {
      insert_suite(f_pTestRegistry, pRetValue);
    }
  }

  CU_set_error(error);
  return pRetValue;
}

CU_pTest
CU_add_test(CU_pSuite pSuite, const char* strName, CU_TestFunc pTestFunc)
{
  CU_pTest pRetValue = NULL;
  CU_ErrorCode error = CUE_SUCCESS;

  assert(CU_FALSE == CU_is_test_running());

  if (NULL == f_pTestRegistry) {
    error = CUE_NOREGISTRY;
  }
  else if (NULL == pSuite) {
    error = CUE_NOSUITE;
  }
  else if (NULL == strName) {
    error = CUE_NO_TESTNAME;
  }
   else if(NULL == pTestFunc) {
    error = CUE_NOTEST;
  }
  else if (CU_TRUE == test_exists(pSuite, strName)) {
    error = CUE_DUP_TEST;
  }
  else {
    pRetValue = create_test(strName, pTestFunc);
    if (NULL == pRetValue) {
      error = CUE_NOMEMORY;
    }
    else {
      f_pTestRegistry->uiNumberOfTests++;
      insert_test(pSuite, pRetValue);
    }
  }

  CU_set_error(error);
  return pRetValue;
}

CU_ErrorCode
CU_register_nsuites(int suite_count, ...)
{
  CU_SuiteInfo *pSuiteItem = NULL;
  CU_TestInfo  *pTestItem = NULL;
  CU_pSuite     pSuite = NULL;

  va_list argptr;
  int i;

  va_start(argptr, suite_count);

  for (i=0 ; i<suite_count ; ++i) {
    pSuiteItem = va_arg(argptr, CU_pSuiteInfo);
    if (NULL != pSuiteItem) {
      for ( ; NULL != pSuiteItem->pName; pSuiteItem++) {
        if (NULL != (pSuite = CU_add_suite(pSuiteItem->pName, pSuiteItem->pInitFunc, pSuiteItem->pCleanupFunc))) {
          for (pTestItem = pSuiteItem->pTests; NULL != pTestItem->pName; pTestItem++) {
            if (NULL == CU_add_test(pSuite, pTestItem->pName, pTestItem->pTestFunc)) {
              return CU_get_error();
            }
          }
        }
        else {
          return CU_get_error();
        }
      }
    }
  }
  return CU_get_error();
}

CU_ErrorCode
CU_register_suites(CU_SuiteInfo suite_info[])
{
  return CU_register_nsuites(1, suite_info);
}

static void
cleanup_test_registry(CU_pTestRegistry pRegistry)
{
  CU_pSuite pCurSuite = NULL;
  CU_pSuite pNextSuite = NULL;

  assert(NULL != pRegistry);

  pCurSuite = pRegistry->pSuite;
  while (NULL != pCurSuite) {
    pNextSuite = pCurSuite->pNext;
    cleanup_suite(pCurSuite);

    CU_FREE(pCurSuite);
    pCurSuite = pNextSuite;
  }
  pRegistry->pSuite = NULL;
  pRegistry->uiNumberOfSuites = 0;
  pRegistry->uiNumberOfTests = 0;
}

static CU_pSuite
create_suite(const char* strName, CU_InitializeFunc pInit,
             CU_CleanupFunc pClean)
{
  CU_pSuite pRetValue = (CU_pSuite)CU_MALLOC(sizeof(CU_Suite));

  assert(NULL != strName);

  if (NULL != pRetValue) {
    pRetValue->pName = (char *)CU_MALLOC(strlen(strName)+1);
    if (NULL != pRetValue->pName) {
      strcpy(pRetValue->pName, strName);
      pRetValue->pInitializeFunc = pInit;
      pRetValue->pCleanupFunc = pClean;
      pRetValue->pTest = NULL;
      pRetValue->pNext = NULL;
      pRetValue->pPrev = NULL;
      pRetValue->uiNumberOfTests = 0;
    }
    else {
      CU_FREE(pRetValue);
      pRetValue = NULL;
    }
  }

  return pRetValue;
}

static void
cleanup_suite(CU_pSuite pSuite)
{
  CU_pTest pCurTest = NULL;
  CU_pTest pNextTest = NULL;

  assert(NULL != pSuite);

  pCurTest = pSuite->pTest;
  while (NULL != pCurTest) {
    pNextTest = pCurTest->pNext;

    cleanup_test(pCurTest);

    CU_FREE(pCurTest);
    pCurTest = pNextTest;
  }
  if (NULL != pSuite->pName) {
    CU_FREE(pSuite->pName);
  }

  pSuite->pName = NULL;
  pSuite->pTest = NULL;
  pSuite->uiNumberOfTests = 0;
}

static void
insert_suite(CU_pTestRegistry pRegistry, CU_pSuite pSuite)
{
  CU_pSuite pCurSuite = NULL;

  assert(NULL != pRegistry);
  assert(NULL != pSuite);

  pCurSuite = pRegistry->pSuite;

  assert(pCurSuite != pSuite);

  pSuite->pNext = NULL;
  pRegistry->uiNumberOfSuites++;

  if (NULL == pCurSuite) {
    pRegistry->pSuite = pSuite;
    pSuite->pPrev = NULL;
  }
  else {
    while (NULL != pCurSuite->pNext) {
      pCurSuite = pCurSuite->pNext;
      assert(pCurSuite != pSuite);
    }

    pCurSuite->pNext = pSuite;
    pSuite->pPrev = pCurSuite;
  }
}

static CU_pTest
create_test(const char* strName, CU_TestFunc pTestFunc)
{
  CU_pTest pRetValue = (CU_pTest)CU_MALLOC(sizeof(CU_Test));

  assert(NULL != strName);

  if (NULL != pRetValue) {
    pRetValue->pName = (char *)CU_MALLOC(strlen(strName)+1);
    if (NULL != pRetValue->pName) {
      strcpy(pRetValue->pName, strName);
      pRetValue->pTestFunc = pTestFunc;
      pRetValue->pJumpBuf = NULL;
      pRetValue->pNext = NULL;
      pRetValue->pPrev = NULL;
    }
    else {
      CU_FREE(pRetValue);
      pRetValue = NULL;
    }
  }

  return pRetValue;
}

static void
cleanup_test(CU_pTest pTest)
{
  assert(NULL != pTest);

  if (NULL != pTest->pName) {
    CU_FREE(pTest->pName);
  }

  pTest->pName = NULL;
}

static void
insert_test(CU_pSuite pSuite, CU_pTest pTest)
{
  CU_pTest pCurTest = NULL;

  assert(NULL != pSuite);
  assert(NULL != pTest);
  assert(NULL == pTest->pNext);
  assert(NULL == pTest->pPrev);

  pCurTest = pSuite->pTest;

  assert(pCurTest != pTest);

  pSuite->uiNumberOfTests++;

  if (NULL == pCurTest) {
    pSuite->pTest = pTest;
    pTest->pPrev = NULL;
  }
  else {
    while (NULL != pCurTest->pNext) {
      pCurTest = pCurTest->pNext;
      assert(pCurTest != pTest);
    }

    pCurTest->pNext = pTest;
    pTest->pPrev = pCurTest;
  }
}

static CU_BOOL
suite_exists(CU_pTestRegistry pRegistry, const char* szSuiteName)
{
  CU_pSuite pSuite = NULL;

  assert(NULL != pRegistry);
  assert(NULL != szSuiteName);

  pSuite = pRegistry->pSuite;
  while (NULL != pSuite) {
    if ((NULL != pSuite->pName) && (0 == CU_compare_strings(szSuiteName, pSuite->pName))) {
      return CU_TRUE;
    }
    pSuite = pSuite->pNext;
  }

  return CU_FALSE;
}

static CU_BOOL
test_exists(CU_pSuite pSuite, const char* szTestName)
{
  CU_pTest pTest = NULL;

  assert(NULL != pSuite);
  assert(NULL != szTestName);

  pTest = pSuite->pTest;
  while (NULL != pTest) {
    if ((NULL != pTest->pName) && (0 == CU_compare_strings(szTestName, pTest->pName))) {
      return CU_TRUE;
    }
    pTest = pTest->pNext;
  }

  return CU_FALSE;
}

CU_pTestRegistry
CU_create_new_registry(void)
{
  CU_pTestRegistry pRegistry = (CU_pTestRegistry)CU_MALLOC(sizeof(CU_TestRegistry));
  if (NULL != pRegistry) {
    pRegistry->pSuite = NULL;
    pRegistry->uiNumberOfSuites = 0;
    pRegistry->uiNumberOfTests = 0;
  }

  return pRegistry;
}

void
CU_destroy_existing_registry(CU_pTestRegistry* ppRegistry)
{
  assert(NULL != ppRegistry);

  if (NULL != *ppRegistry) {
    cleanup_test_registry(*ppRegistry);
  }
  CU_FREE(*ppRegistry);
  *ppRegistry = NULL;
}

CU_pSuite
CU_get_suite_by_name(const char* szSuiteName, CU_pTestRegistry pRegistry)
{
  CU_pSuite pSuite = NULL;
  CU_pSuite pCur = NULL;

  assert(NULL != pRegistry);
  assert(NULL != szSuiteName);

  pCur = pRegistry->pSuite;
  while (NULL != pCur)  {
    if ((NULL != pCur->pName) && (0 == CU_compare_strings(pCur->pName, szSuiteName))) {
      pSuite = pCur;
      break;
    }
    pCur = pCur->pNext;
  }

  return pSuite;
}

CU_pTest
CU_get_test_by_name(const char* szTestName, CU_pSuite pSuite)
{
  CU_pTest pTest = NULL;
  CU_pTest pCur = NULL;

  assert(NULL != pSuite);
  assert(NULL != szTestName);

  pCur = pSuite->pTest;
  while (NULL != pCur) {
    if ((NULL != pCur->pName) && (0 == CU_compare_strings(pCur->pName, szTestName))) {
      pTest = pCur;
      break;
    }
    pCur = pCur->pNext;
  }

  return pTest;
}

#ifdef CUNIT_BUILD_TESTS
#include "test_cunit.h"

static int
sfunc1(void)
{
  return 0;
}

static void
test1(void)
{
}

static void
test_CU_initialize_registry(void)
{
  CU_pTestRegistry pReg = NULL;
  unsigned int ndeallocs_before;

  TEST(NULL == CU_get_registry());
  TEST(CU_FALSE == CU_registry_initialized());

  TEST(CUE_SUCCESS == CU_initialize_registry());
  pReg = CU_get_registry();
  TEST_FATAL(NULL != pReg);
  TEST(CU_TRUE == CU_registry_initialized());
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(NULL == pReg->pSuite);

  TEST(0 < test_cunit_get_n_memevents(pReg));
  ndeallocs_before = test_cunit_get_n_deallocations(pReg);
  TEST(CUE_SUCCESS == CU_initialize_registry());
  TEST((ndeallocs_before + 1) == test_cunit_get_n_deallocations(pReg));
  pReg = CU_get_registry();
  TEST_FATAL(NULL != pReg);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(NULL == pReg->pSuite);

  CU_cleanup_registry();
  TEST(NULL == CU_get_registry());
  TEST(CU_FALSE == CU_registry_initialized());

  test_cunit_deactivate_malloc();
  TEST(CUE_NOMEMORY == CU_initialize_registry());
  TEST(NULL == CU_get_registry());
  TEST(CU_FALSE == CU_registry_initialized());
  test_cunit_activate_malloc();
}

static void
test_CU_cleanup_registry(void)
{
  CU_cleanup_registry();
  CU_cleanup_registry();
  CU_cleanup_registry();
  CU_cleanup_registry();
  CU_cleanup_registry();
}

static void
test_CU_add_suite(void)
{
  CU_pSuite pSuite = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pSuite pSuite3 = NULL;
  CU_pSuite pSuite4 = NULL;
  CU_pTestRegistry pReg = NULL;

  CU_cleanup_registry();

  pSuite = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_NOREGISTRY == CU_get_error());
  TEST(NULL == pSuite);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite = CU_add_suite(NULL, NULL, NULL);
  TEST(CUE_NO_SUITENAME == CU_get_error());
  TEST(NULL == pSuite);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pSuite);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  pSuite = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_DUP_SUITE == CU_get_error());
  TEST(NULL == pSuite);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  CU_initialize_registry();
  pReg = CU_get_registry();

  test_cunit_deactivate_malloc();
  pSuite = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_NOMEMORY == CU_get_error());
  TEST(NULL == pSuite);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  test_cunit_activate_malloc();

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pSuite);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(CU_get_suite_by_name("suite1", pReg) == pSuite);
  TEST(pReg->pSuite == pSuite);

  TEST(!strcmp("suite1", pSuite->pName));
  TEST(pSuite->pTest == NULL);            /* no tests added yet */
  TEST(pSuite->uiNumberOfTests == 0);     /* no tests added yet */
  TEST(pSuite->pInitializeFunc == NULL);  /* no init function */
  TEST(pSuite->pCleanupFunc == NULL);     /* no cleanup function */
  TEST(pSuite->pNext == NULL);            /* no more suites added yet */

  pSuite2 = CU_add_suite("suite2", sfunc1, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pSuite2);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(CU_get_suite_by_name("suite2", pReg) == pSuite2);

  pSuite3 = CU_add_suite("suite3", NULL, sfunc1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pSuite3);
  TEST(3 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(CU_get_suite_by_name("suite3", pReg) == pSuite3);

  pSuite4 = CU_add_suite("suite4", sfunc1, sfunc1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pSuite4);
  TEST(4 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(CU_get_suite_by_name("suite4", pReg) == pSuite4);

  TEST(pReg->pSuite == pSuite);

  TEST(!strcmp("suite1", pSuite->pName));
  TEST(pSuite->pTest == NULL);              /* no tests added yet */
  TEST(pSuite->uiNumberOfTests == 0);       /* no tests added yet */
  TEST(pSuite->pInitializeFunc == NULL);    /* no init function */
  TEST(pSuite->pCleanupFunc == NULL);       /* no cleanup function */
  TEST(pSuite->pNext == pSuite2);           /* now have another suite */

  TEST(!strcmp("suite2", pSuite2->pName));
  TEST(pSuite2->pTest == NULL);             /* no tests added yet */
  TEST(pSuite2->uiNumberOfTests == 0);      /* no tests added yet */
  TEST(pSuite2->pInitializeFunc == sfunc1); /* no init function */
  TEST(pSuite2->pCleanupFunc == NULL);      /* no cleanup function */
  TEST(pSuite2->pNext == pSuite3);          /* next suite in list */

  TEST(!strcmp("suite3", pSuite3->pName));
  TEST(pSuite3->pTest == NULL);             /* no tests added yet */
  TEST(pSuite3->uiNumberOfTests == 0);      /* no tests added yet */
  TEST(pSuite3->pInitializeFunc == NULL);   /* no init function */
  TEST(pSuite3->pCleanupFunc == sfunc1);    /* no cleanup function */
  TEST(pSuite3->pNext == pSuite4);          /* next suite in list */

  TEST(!strcmp("suite4", pSuite4->pName));
  TEST(pSuite4->pTest == NULL);             /* no tests added yet */
  TEST(pSuite4->uiNumberOfTests == 0);      /* no tests added yet */
  TEST(pSuite4->pInitializeFunc == sfunc1); /* no init function */
  TEST(pSuite4->pCleanupFunc == sfunc1);    /* no cleanup function */
  TEST(pSuite4->pNext == NULL);             /* end of suite list */

  TEST(0 != test_cunit_get_n_memevents(pSuite));
  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(0 != test_cunit_get_n_memevents(pSuite3));
  TEST(0 != test_cunit_get_n_memevents(pSuite4));

  TEST(test_cunit_get_n_allocations(pSuite) != test_cunit_get_n_deallocations(pSuite));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pSuite3) != test_cunit_get_n_deallocations(pSuite3));
  TEST(test_cunit_get_n_allocations(pSuite4) != test_cunit_get_n_deallocations(pSuite4));

  CU_cleanup_registry();

  TEST(test_cunit_get_n_allocations(pSuite) == test_cunit_get_n_deallocations(pSuite));
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pSuite3) == test_cunit_get_n_deallocations(pSuite3));
  TEST(test_cunit_get_n_allocations(pSuite4) == test_cunit_get_n_deallocations(pSuite4));
}

static void
test_CU_add_test(void)
{
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pTest pTest1 = NULL;
  CU_pTest pTest2 = NULL;
  CU_pTest pTest3 = NULL;
  CU_pTest pTest4 = NULL;
  CU_pTestRegistry pReg = NULL;

  CU_cleanup_registry();

  pTest1 = CU_add_test(pSuite1, "test1", test1);
  TEST(CUE_NOREGISTRY == CU_get_error());
  TEST(NULL == pTest1);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pTest1 = CU_add_test(pSuite1, "test1", test1);
  TEST(CUE_NOSUITE == CU_get_error());
  TEST(NULL == pTest1);

  pSuite1 = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  pTest1 = CU_add_test(pSuite1, NULL, test1);
  TEST(CUE_NO_TESTNAME == CU_get_error());
  TEST(NULL == pTest1);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(0 == pSuite1->uiNumberOfTests);

  pTest1 = CU_add_test(pSuite1, "test1", NULL);
  TEST(CUE_NOTEST == CU_get_error());
  TEST(NULL == pTest1);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(0 == pSuite1->uiNumberOfTests);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite1 = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  pTest1 = CU_add_test(pSuite1, "test1", test1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pTest1);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(1 == pReg->uiNumberOfTests);
  TEST(1 == pSuite1->uiNumberOfTests);

  pTest2 = CU_add_test(pSuite1, "test1", test1);
  TEST(CUE_DUP_TEST == CU_get_error());
  TEST(NULL == pTest2);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(1 == pReg->uiNumberOfTests);
  TEST(1 == pSuite1->uiNumberOfTests);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite1 = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  test_cunit_deactivate_malloc();
  pTest1 = CU_add_test(pSuite1, "test1", test1);
  test_cunit_activate_malloc();
  TEST(CUE_NOMEMORY == CU_get_error());
  TEST(NULL == pTest1);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(0 == pSuite1->uiNumberOfTests);

  CU_initialize_registry();
  pReg = CU_get_registry();

  pSuite1 = CU_add_suite("suite1", NULL, NULL);
  TEST(CUE_SUCCESS == CU_get_error());
  pSuite2 = CU_add_suite("suite2", sfunc1, sfunc1);
  TEST(CUE_SUCCESS == CU_get_error());

  pTest1 = CU_add_test(pSuite1, "test1", test1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pTest1);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(1 == pReg->uiNumberOfTests);
  TEST(1 == pSuite1->uiNumberOfTests);
  TEST(0 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == NULL);

  pTest2 = CU_add_test(pSuite2, "test2", test1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pTest2);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(2 == pReg->uiNumberOfTests);
  TEST(1 == pSuite1->uiNumberOfTests);
  TEST(1 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == pTest2);

  pTest3 = CU_add_test(pSuite1, "test3", test1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pTest3);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(3 == pReg->uiNumberOfTests);
  TEST(2 == pSuite1->uiNumberOfTests);
  TEST(1 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == pTest2);

  pTest4 = CU_add_test(pSuite1, "test4", test1);
  TEST(CUE_SUCCESS == CU_get_error());
  TEST(NULL != pTest4);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(4 == pReg->uiNumberOfTests);
  TEST(3 == pSuite1->uiNumberOfTests);
  TEST(1 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == pTest2);

  TEST(!strcmp("test1", pTest1->pName));
  TEST(pTest1->pNext == pTest3);
  TEST(pTest1->pJumpBuf == NULL);
  TEST(pTest1->pTestFunc == test1);
  TEST(CU_get_test_by_name("test1", pSuite1) == pTest1);
  TEST(CU_get_test_by_name("test1", pSuite2) == NULL);

  TEST(!strcmp("test2", pTest2->pName));
  TEST(pTest2->pNext == NULL);
  TEST(pTest2->pJumpBuf == NULL);
  TEST(pTest2->pTestFunc == test1);
  TEST(CU_get_test_by_name("test2", pSuite1) == NULL);
  TEST(CU_get_test_by_name("test2", pSuite2) == pTest2);

  TEST(!strcmp("test3", pTest3->pName));
  TEST(pTest3->pNext == pTest4);
  TEST(pTest3->pJumpBuf == NULL);
  TEST(pTest3->pTestFunc == test1);
  TEST(CU_get_test_by_name("test3", pSuite1) == pTest3);
  TEST(CU_get_test_by_name("test3", pSuite2) == NULL);

  TEST(!strcmp("test4", pTest4->pName));
  TEST(pTest4->pNext == NULL);
  TEST(pTest4->pJumpBuf == NULL);
  TEST(pTest4->pTestFunc == test1);
  TEST(CU_get_test_by_name("test4", pSuite1) == pTest4);
  TEST(CU_get_test_by_name("test4", pSuite2) == NULL);

  TEST(0 != test_cunit_get_n_memevents(pSuite1));
  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(0 != test_cunit_get_n_memevents(pTest1));
  TEST(0 != test_cunit_get_n_memevents(pTest2));
  TEST(0 != test_cunit_get_n_memevents(pTest3));
  TEST(0 != test_cunit_get_n_memevents(pTest4));

  TEST(test_cunit_get_n_allocations(pSuite1) != test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) != test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) != test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) != test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) != test_cunit_get_n_deallocations(pTest4));

  CU_cleanup_registry();

  TEST(test_cunit_get_n_allocations(pSuite1) == test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) == test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) == test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) == test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) == test_cunit_get_n_deallocations(pTest4));
}

static void
test_CU_get_registry(void)
{
  CU_cleanup_registry();
  TEST(NULL == CU_get_registry());

  CU_initialize_registry();
  TEST(NULL != CU_get_registry());
  TEST(f_pTestRegistry == CU_get_registry());

  CU_cleanup_registry();
}

static void
test_CU_set_registry(void)
{
  CU_pTestRegistry pReg1 = NULL;
  CU_pTestRegistry pReg2 = NULL;
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;

  CU_initialize_registry();
  pSuite1 = CU_add_suite("suite1", NULL, NULL);
  pSuite2 = CU_add_suite("suite2", NULL, NULL);

  CU_add_test(pSuite1, "test1", test1);
  CU_add_test(pSuite1, "test2", test1);
  CU_add_test(pSuite2, "test1", test1);
  CU_add_test(pSuite2, "test2", test1);

  pReg1 = CU_get_registry();

  TEST(pReg1->pSuite == pSuite1);
  TEST(pReg1->uiNumberOfSuites == 2);
  TEST(pReg1->uiNumberOfTests == 4);
  TEST(0 < test_cunit_get_n_memevents(pReg1));
  TEST(test_cunit_get_n_allocations(pReg1) != test_cunit_get_n_deallocations(pReg1));

  CU_set_registry(NULL);

  TEST(test_cunit_get_n_allocations(pReg1) != test_cunit_get_n_deallocations(pReg1));

  CU_cleanup_registry();

  TEST(test_cunit_get_n_allocations(pReg1) != test_cunit_get_n_deallocations(pReg1));

  pReg2 = CU_create_new_registry();
  CU_set_registry(pReg2);

  TEST(pReg1->pSuite == pSuite1);
  TEST(pReg1->uiNumberOfSuites == 2);
  TEST(pReg1->uiNumberOfTests == 4);
  TEST(test_cunit_get_n_allocations(pReg1) != test_cunit_get_n_deallocations(pReg1));

  TEST(CU_get_registry()->pSuite == NULL);
  TEST(CU_get_registry()->uiNumberOfSuites == 0);
  TEST(CU_get_registry()->uiNumberOfTests == 0);
  TEST(0 < test_cunit_get_n_memevents(pReg2));
  TEST(test_cunit_get_n_allocations(pReg2) != test_cunit_get_n_deallocations(pReg2));

  CU_cleanup_registry();

  TEST(pReg1->pSuite == pSuite1);
  TEST(pReg1->uiNumberOfSuites == 2);
  TEST(pReg1->uiNumberOfTests == 4);
  TEST(test_cunit_get_n_allocations(pReg1) != test_cunit_get_n_deallocations(pReg1));
  TEST(test_cunit_get_n_allocations(pReg2) == test_cunit_get_n_deallocations(pReg2));

  CU_set_registry(pReg1);
  CU_cleanup_registry();
  TEST(test_cunit_get_n_allocations(pReg1) == test_cunit_get_n_deallocations(pReg1));
}

static void
test_CU_create_new_registry(void)
{
  CU_pTestRegistry pReg = NULL;
  CU_pTestRegistry pRegOld = NULL;

  CU_cleanup_registry();
  pReg = CU_create_new_registry();

  TEST(NULL != pReg);
  TEST(0 < test_cunit_get_n_memevents(pReg));
  TEST(test_cunit_get_n_allocations(pReg) != test_cunit_get_n_deallocations(pReg));

  TEST(pReg->pSuite == NULL);
  TEST(pReg->uiNumberOfSuites == 0);
  TEST(pReg->uiNumberOfTests == 0);

  CU_cleanup_registry();
  TEST(test_cunit_get_n_allocations(pReg) != test_cunit_get_n_deallocations(pReg));

  pRegOld = pReg;
  CU_destroy_existing_registry(&pReg);
  TEST(test_cunit_get_n_allocations(pRegOld) == test_cunit_get_n_deallocations(pRegOld));
  TEST(NULL == pReg);
}

static void
test_CU_destroy_existing_registry(void)
{
}

static void
test_CU_get_suite_by_name(void)
{
}

static void
test_CU_get_test_by_name(void)
{
}

static void
test_cleanup_test_registry(void)
{
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pTest pTest1 = NULL;
  CU_pTest pTest2 = NULL;
  CU_pTest pTest3 = NULL;
  CU_pTest pTest4 = NULL;
  CU_pTestRegistry pReg = CU_create_new_registry();

  TEST_FATAL(NULL != pReg);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  pTest1 = create_test("test1", test1);
  pTest2 = create_test("test2", NULL);
  pTest3 = create_test("test3", test1);
  pTest4 = create_test("", NULL);

  pSuite1 = create_suite("suite1", NULL, NULL);
  pSuite2 = create_suite("suite2", sfunc1, sfunc1);
  insert_suite(pReg, pSuite1);
  insert_suite(pReg, pSuite2);

  insert_test(pSuite1, pTest1);
  insert_test(pSuite1, pTest2);
  insert_test(pSuite1, pTest3);
  insert_test(pSuite2, pTest4);

  TEST(2 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);   /* not managed in primitive functions */
  TEST(3 == pSuite1->uiNumberOfTests);
  TEST(1 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == pTest4);
  TEST(pTest1->pNext == pTest2);
  TEST(pTest1->pPrev == NULL);
  TEST(pTest2->pNext == pTest3);
  TEST(pTest2->pPrev == pTest1);
  TEST(pTest3->pNext == NULL);
  TEST(pTest3->pPrev == pTest2);
  TEST(pTest4->pNext == NULL);
  TEST(pTest4->pPrev == NULL);

  TEST(0 != test_cunit_get_n_memevents(pReg));
  TEST(0 != test_cunit_get_n_memevents(pSuite1));
  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(0 != test_cunit_get_n_memevents(pTest1));
  TEST(0 != test_cunit_get_n_memevents(pTest2));
  TEST(0 != test_cunit_get_n_memevents(pTest3));
  TEST(0 != test_cunit_get_n_memevents(pTest4));

  TEST(test_cunit_get_n_allocations(pReg) != test_cunit_get_n_deallocations(pReg));
  TEST(test_cunit_get_n_allocations(pSuite1) != test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) != test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) != test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) != test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) != test_cunit_get_n_deallocations(pTest4));

  cleanup_test_registry(pReg);
  CU_FREE(pReg);

  TEST(test_cunit_get_n_allocations(pReg) == test_cunit_get_n_deallocations(pReg));
  TEST(test_cunit_get_n_allocations(pSuite1) == test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) == test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) == test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) == test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) == test_cunit_get_n_deallocations(pTest4));
}

static void
test_create_suite(void)
{
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pSuite pSuite3 = NULL;
  CU_pSuite pSuite4 = NULL;

  test_cunit_deactivate_malloc();
  pSuite1 = create_suite("suite1", NULL, NULL);
  TEST(NULL == pSuite1);
  test_cunit_activate_malloc();

  pSuite1 = create_suite("suite1", NULL, NULL);
  TEST(NULL != pSuite1);
  TEST(!strcmp("suite1", pSuite1->pName));
  TEST(pSuite1->pTest == NULL);            /* no tests added yet */
  TEST(pSuite1->uiNumberOfTests == 0);     /* no tests added yet */
  TEST(pSuite1->pInitializeFunc == NULL);  /* no init function */
  TEST(pSuite1->pCleanupFunc == NULL);     /* no cleanup function */
  TEST(pSuite1->pNext == NULL);            /* no more suites added yet */

  pSuite2 = create_suite("suite2", sfunc1, NULL);
  TEST(NULL != pSuite2);
  TEST(!strcmp("suite2", pSuite2->pName));
  TEST(pSuite2->pTest == NULL);             /* no tests added yet */
  TEST(pSuite2->uiNumberOfTests == 0);      /* no tests added yet */
  TEST(pSuite2->pInitializeFunc == sfunc1); /* init function */
  TEST(pSuite2->pCleanupFunc == NULL);      /* no cleanup function */
  TEST(pSuite2->pNext == NULL);             /* no more suites added yet */

  pSuite3 = create_suite("suite3", NULL, sfunc1);
  TEST(NULL != pSuite3);
  TEST(!strcmp("suite3", pSuite3->pName));
  TEST(pSuite3->pTest == NULL);            /* no tests added yet */
  TEST(pSuite3->uiNumberOfTests == 0);     /* no tests added yet */
  TEST(pSuite3->pInitializeFunc == NULL);  /* no init function */
  TEST(pSuite3->pCleanupFunc == sfunc1);   /* cleanup function */
  TEST(pSuite3->pNext == NULL);            /* no more suites added yet */

  pSuite4 = create_suite("suite4", sfunc1, sfunc1);
  TEST(NULL != pSuite4);
  TEST(!strcmp("suite4", pSuite4->pName));
  TEST(pSuite4->pTest == NULL);             /* no tests added yet */
  TEST(pSuite4->uiNumberOfTests == 0);      /* no tests added yet */
  TEST(pSuite4->pInitializeFunc == sfunc1); /* no init function */
  TEST(pSuite4->pCleanupFunc == sfunc1);    /* cleanup function */
  TEST(pSuite4->pNext == NULL);             /* no more suites added yet */

  TEST(0 != test_cunit_get_n_memevents(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite1) != test_cunit_get_n_deallocations(pSuite1));
  cleanup_suite(pSuite1);
  CU_FREE(pSuite1);
  TEST(test_cunit_get_n_allocations(pSuite1) == test_cunit_get_n_deallocations(pSuite1));

  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  cleanup_suite(pSuite2);
  CU_FREE(pSuite2);
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));

  TEST(0 != test_cunit_get_n_memevents(pSuite3));
  TEST(test_cunit_get_n_allocations(pSuite3) != test_cunit_get_n_deallocations(pSuite3));
  cleanup_suite(pSuite3);
  CU_FREE(pSuite3);
  TEST(test_cunit_get_n_allocations(pSuite3) == test_cunit_get_n_deallocations(pSuite3));

  TEST(0 != test_cunit_get_n_memevents(pSuite4));
  TEST(test_cunit_get_n_allocations(pSuite4) != test_cunit_get_n_deallocations(pSuite4));
  cleanup_suite(pSuite4);
  CU_FREE(pSuite4);
  TEST(test_cunit_get_n_allocations(pSuite4) == test_cunit_get_n_deallocations(pSuite4));
}

static void
test_cleanup_suite(void)
{
}

static void
test_insert_suite(void)
{
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pSuite pSuite3 = NULL;
  CU_pSuite pSuite4 = NULL;
  CU_pTestRegistry pReg = CU_create_new_registry();

  TEST_FATAL(NULL != pReg);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(NULL == pReg->pSuite);
  TEST(CU_FALSE == suite_exists(pReg, "suite1"));
  TEST(CU_FALSE == suite_exists(pReg, "suite2"));
  TEST(CU_FALSE == suite_exists(pReg, "suite3"));
  TEST(CU_FALSE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));

  pSuite1 = create_suite("suite1", NULL, NULL);
  insert_suite(pReg, pSuite1);
  TEST(1 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(pReg->pSuite == pSuite1);
  TEST(pSuite1->pNext == NULL);
  TEST(CU_TRUE == suite_exists(pReg, "suite1"));
  TEST(CU_FALSE == suite_exists(pReg, "suite2"));
  TEST(CU_FALSE == suite_exists(pReg, "suite3"));
  TEST(CU_FALSE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));

  pSuite2 = create_suite("suite2", sfunc1, NULL);
  insert_suite(pReg, pSuite2);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(pReg->pSuite == pSuite1);
  TEST(pSuite1->pNext == pSuite2);
  TEST(pSuite2->pNext == NULL);
  TEST(CU_TRUE == suite_exists(pReg, "suite1"));
  TEST(CU_TRUE == suite_exists(pReg, "suite2"));
  TEST(CU_FALSE == suite_exists(pReg, "suite3"));
  TEST(CU_FALSE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));

  pSuite3 = create_suite("suite3", NULL, sfunc1);
  insert_suite(pReg, pSuite3);
  TEST(3 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(pReg->pSuite == pSuite1);
  TEST(pSuite1->pNext == pSuite2);
  TEST(pSuite2->pNext == pSuite3);
  TEST(pSuite3->pNext == NULL);
  TEST(CU_TRUE == suite_exists(pReg, "suite1"));
  TEST(CU_TRUE == suite_exists(pReg, "suite2"));
  TEST(CU_TRUE == suite_exists(pReg, "suite3"));
  TEST(CU_FALSE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));

  pSuite4 = create_suite("suite4", sfunc1, sfunc1);
  insert_suite(pReg, pSuite4);
  TEST(4 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);
  TEST(pReg->pSuite == pSuite1);
  TEST(pSuite1->pNext == pSuite2);
  TEST(pSuite2->pNext == pSuite3);
  TEST(pSuite3->pNext == pSuite4);
  TEST(pSuite4->pNext == NULL);
  TEST(CU_TRUE == suite_exists(pReg, "suite1"));
  TEST(CU_TRUE == suite_exists(pReg, "suite2"));
  TEST(CU_TRUE == suite_exists(pReg, "suite3"));
  TEST(CU_TRUE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));

  TEST(0 != test_cunit_get_n_memevents(pReg));
  TEST(0 != test_cunit_get_n_memevents(pSuite1));
  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(0 != test_cunit_get_n_memevents(pSuite3));
  TEST(0 != test_cunit_get_n_memevents(pSuite4));

  TEST(test_cunit_get_n_allocations(pReg) != test_cunit_get_n_deallocations(pReg));
  TEST(test_cunit_get_n_allocations(pSuite1) != test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pSuite3) != test_cunit_get_n_deallocations(pSuite3));
  TEST(test_cunit_get_n_allocations(pSuite4) != test_cunit_get_n_deallocations(pSuite4));

  cleanup_test_registry(pReg);
  TEST(CU_FALSE == suite_exists(pReg, "suite1"));
  TEST(CU_FALSE == suite_exists(pReg, "suite2"));
  TEST(CU_FALSE == suite_exists(pReg, "suite3"));
  TEST(CU_FALSE == suite_exists(pReg, "suite4"));
  TEST(CU_FALSE == suite_exists(pReg, "suite5"));
  TEST(CU_FALSE == suite_exists(pReg, ""));
  CU_FREE(pReg);

  TEST(test_cunit_get_n_allocations(pReg) == test_cunit_get_n_deallocations(pReg));
  TEST(test_cunit_get_n_allocations(pSuite1) == test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pSuite3) == test_cunit_get_n_deallocations(pSuite3));
  TEST(test_cunit_get_n_allocations(pSuite4) == test_cunit_get_n_deallocations(pSuite4));
}

static void
test_create_test(void)
{
  CU_pTest pTest1 = NULL;
  CU_pTest pTest2 = NULL;

  test_cunit_deactivate_malloc();
  pTest1 = create_test("test1", test1);
  test_cunit_activate_malloc();
  TEST(NULL == pTest1);

  pTest1 = create_test("test1", test1);
  TEST(NULL != pTest1);
  TEST(pTest1->pTestFunc == test1);
  TEST(!strcmp("test1", pTest1->pName));
  TEST(pTest1->pNext == NULL);
  TEST(pTest1->pPrev == NULL);
  TEST(pTest1->pJumpBuf == NULL);

  pTest2= create_test("test2", NULL);
  TEST(NULL != pTest2);
  TEST(pTest2->pTestFunc == NULL);
  TEST(!strcmp("test2", pTest2->pName));
  TEST(pTest2->pNext == NULL);
  TEST(pTest2->pPrev == NULL);
  TEST(pTest2->pJumpBuf == NULL);

  TEST(0 != test_cunit_get_n_memevents(pTest1));
  TEST(0 != test_cunit_get_n_memevents(pTest2));

  TEST(test_cunit_get_n_allocations(pTest1) != test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) != test_cunit_get_n_deallocations(pTest2));

  cleanup_test(pTest1);
  CU_FREE(pTest1);
  cleanup_test(pTest2);
  CU_FREE(pTest2);

  TEST(test_cunit_get_n_allocations(pTest1) == test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) == test_cunit_get_n_deallocations(pTest2));
}

static void
test_insert_test(void)
{
  CU_pSuite pSuite1 = NULL;
  CU_pSuite pSuite2 = NULL;
  CU_pTest pTest1 = NULL;
  CU_pTest pTest2 = NULL;
  CU_pTest pTest3 = NULL;
  CU_pTest pTest4 = NULL;

  pTest1 = create_test("test1", test1);
  pTest2 = create_test("test2", NULL);
  pTest3 = create_test("test3", test1);
  pTest4 = create_test("", NULL);

  pSuite1 = create_suite("suite1", NULL, NULL);
  pSuite2 = create_suite("suite2", sfunc1, sfunc1);

  TEST(CU_FALSE == test_exists(pSuite1, "test1"));
  TEST(CU_FALSE == test_exists(pSuite1, "test2"));
  TEST(CU_FALSE == test_exists(pSuite1, "test3"));
  TEST(CU_FALSE == test_exists(pSuite1, "test4"));
  TEST(CU_FALSE == test_exists(pSuite1, ""));
  TEST(CU_FALSE == test_exists(pSuite2, "test1"));
  TEST(CU_FALSE == test_exists(pSuite2, "test2"));
  TEST(CU_FALSE == test_exists(pSuite2, "test3"));
  TEST(CU_FALSE == test_exists(pSuite2, "test4"));
  TEST(CU_FALSE == test_exists(pSuite2, ""));

  insert_test(pSuite1, pTest1);
  insert_test(pSuite1, pTest2);
  insert_test(pSuite1, pTest3);
  insert_test(pSuite2, pTest4);

  TEST(CU_TRUE == test_exists(pSuite1, "test1"));
  TEST(CU_TRUE == test_exists(pSuite1, "test2"));
  TEST(CU_TRUE == test_exists(pSuite1, "test3"));
  TEST(CU_FALSE == test_exists(pSuite1, "test4"));
  TEST(CU_FALSE == test_exists(pSuite1, ""));
  TEST(CU_FALSE == test_exists(pSuite2, "test1"));
  TEST(CU_FALSE == test_exists(pSuite2, "test2"));
  TEST(CU_FALSE == test_exists(pSuite2, "test3"));
  TEST(CU_FALSE == test_exists(pSuite2, "test4"));
  TEST(CU_TRUE == test_exists(pSuite2, ""));

  TEST(3 == pSuite1->uiNumberOfTests);
  TEST(1 == pSuite2->uiNumberOfTests);
  TEST(pSuite1->pTest == pTest1);
  TEST(pSuite2->pTest == pTest4);
  TEST(pTest1->pNext == pTest2);
  TEST(pTest1->pPrev == NULL);
  TEST(pTest2->pNext == pTest3);
  TEST(pTest2->pPrev == pTest1);
  TEST(pTest3->pNext == NULL);
  TEST(pTest3->pPrev == pTest2);
  TEST(pTest4->pNext == NULL);
  TEST(pTest4->pPrev == NULL);

  TEST(0 != test_cunit_get_n_memevents(pSuite1));
  TEST(0 != test_cunit_get_n_memevents(pSuite2));
  TEST(0 != test_cunit_get_n_memevents(pTest1));
  TEST(0 != test_cunit_get_n_memevents(pTest2));
  TEST(0 != test_cunit_get_n_memevents(pTest3));
  TEST(0 != test_cunit_get_n_memevents(pTest4));

  TEST(test_cunit_get_n_allocations(pSuite1) != test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) != test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) != test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) != test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) != test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) != test_cunit_get_n_deallocations(pTest4));

  cleanup_suite(pSuite1);

  TEST(CU_FALSE == test_exists(pSuite1, "test1"));
  TEST(CU_FALSE == test_exists(pSuite1, "test2"));
  TEST(CU_FALSE == test_exists(pSuite1, "test3"));
  TEST(CU_FALSE == test_exists(pSuite1, "test4"));
  TEST(CU_FALSE == test_exists(pSuite1, ""));
  TEST(CU_FALSE == test_exists(pSuite2, "test1"));
  TEST(CU_FALSE == test_exists(pSuite2, "test2"));
  TEST(CU_FALSE == test_exists(pSuite2, "test3"));
  TEST(CU_FALSE == test_exists(pSuite2, "test4"));
  TEST(CU_TRUE == test_exists(pSuite2, ""));

  cleanup_suite(pSuite2);

  TEST(CU_FALSE == test_exists(pSuite1, "test1"));
  TEST(CU_FALSE == test_exists(pSuite1, "test2"));
  TEST(CU_FALSE == test_exists(pSuite1, "test3"));
  TEST(CU_FALSE == test_exists(pSuite1, "test4"));
  TEST(CU_FALSE == test_exists(pSuite1, ""));
  TEST(CU_FALSE == test_exists(pSuite2, "test1"));
  TEST(CU_FALSE == test_exists(pSuite2, "test2"));
  TEST(CU_FALSE == test_exists(pSuite2, "test3"));
  TEST(CU_FALSE == test_exists(pSuite2, "test4"));
  TEST(CU_FALSE == test_exists(pSuite2, ""));

  CU_FREE(pSuite1);
  CU_FREE(pSuite2);

  TEST(test_cunit_get_n_allocations(pSuite1) == test_cunit_get_n_deallocations(pSuite1));
  TEST(test_cunit_get_n_allocations(pSuite2) == test_cunit_get_n_deallocations(pSuite2));
  TEST(test_cunit_get_n_allocations(pTest1) == test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pTest2) == test_cunit_get_n_deallocations(pTest2));
  TEST(test_cunit_get_n_allocations(pTest3) == test_cunit_get_n_deallocations(pTest3));
  TEST(test_cunit_get_n_allocations(pTest4) == test_cunit_get_n_deallocations(pTest4));
}

static void
test_cleanup_test(void)
{
  char* pName;
  CU_pTest pTest1 = create_test("test1", NULL);

  TEST_FATAL(NULL != pTest1);

  pName = pTest1->pName;
  TEST(0 != test_cunit_get_n_memevents(pTest1));
  TEST(0 != test_cunit_get_n_memevents(pName));

  TEST(test_cunit_get_n_allocations(pTest1) != test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pName) != test_cunit_get_n_deallocations(pName));

  cleanup_test(pTest1);
  CU_FREE(pTest1);

  TEST(test_cunit_get_n_allocations(pTest1) == test_cunit_get_n_deallocations(pTest1));
  TEST(test_cunit_get_n_allocations(pName) == test_cunit_get_n_deallocations(pName));
}

static void
test_suite_exists(void)
{
}

static void
test_test_exists(void)
{
}

static
void group_A_case_1(void)
{
  CU_ASSERT_TRUE(1);
}

static void
group_A_case_2(void)
{
  CU_ASSERT_TRUE(2);
}

static void
group_B_case_1(void)
{
  CU_ASSERT_FALSE(1);
}

static void
group_B_case_2(void)
{
  CU_ASSERT_FALSE(2);
}

static CU_TestInfo group_A_test_cases[] = {
  { "1", group_A_case_1 },
  { "2", group_A_case_2 },
  CU_TEST_INFO_NULL,
};

static CU_TestInfo group_B_test_cases[] = {
  { "1", group_B_case_1 },
  { "2", group_B_case_2 },
  CU_TEST_INFO_NULL,
};

static CU_SuiteInfo suites0[] = {
  CU_SUITE_INFO_NULL,
};

static CU_SuiteInfo suites1[] = {
  { "A1", NULL, NULL, group_A_test_cases },
  { "B1", NULL, NULL, group_B_test_cases },
  CU_SUITE_INFO_NULL,
};

static CU_SuiteInfo suites2[] = {
  { "A2", NULL, NULL, group_A_test_cases },
  { "B2", NULL, NULL, group_B_test_cases },
  CU_SUITE_INFO_NULL,
};

static void
test_register_suite(void)
{
  CU_pTestRegistry pReg = NULL;
  CU_ErrorCode status;

  if (CU_initialize_registry()) {
    fprintf(stderr, "\nError initializing registry in test_register_suite().");
    return;
  }

  pReg = CU_get_registry();

  TEST_FATAL(NULL != pReg);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  status = CU_register_suites(NULL);
  TEST(CUE_SUCCESS == status);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  status = CU_register_suites(suites0);
  TEST(CUE_SUCCESS == status);
  TEST(0 == pReg->uiNumberOfSuites);
  TEST(0 == pReg->uiNumberOfTests);

  status = CU_register_suites(suites1);
  TEST(CUE_SUCCESS == status);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(4 == pReg->uiNumberOfTests);

  status = CU_register_suites(suites1);
  TEST(CUE_DUP_SUITE == status);
  TEST(2 == pReg->uiNumberOfSuites);
  TEST(4 == pReg->uiNumberOfTests);

  CU_cleanup_registry();

  if (CU_initialize_registry()) {
    fprintf(stderr, "\nError initializing registry in test_register_suite().");
    return;
  }

  pReg = CU_get_registry();

  status = CU_register_nsuites(2, suites1, suites2);
  TEST(CUE_SUCCESS == status);
  TEST(4 == pReg->uiNumberOfSuites);
  TEST(8 == pReg->uiNumberOfTests);
}

void
test_cunit_TestDB(void)
{
  test_cunit_start_tests("TestDB.c");

  test_CU_initialize_registry();
  test_CU_cleanup_registry();
  test_CU_add_suite();
  test_CU_add_test();
  test_CU_get_registry();
  test_CU_set_registry();
  test_CU_create_new_registry();
  test_CU_destroy_existing_registry();
  test_CU_get_suite_by_name();
  test_CU_get_test_by_name();
  test_cleanup_test_registry();
  test_create_suite();
  test_cleanup_suite();
  test_insert_suite();
  test_create_test();
  test_cleanup_test();
  test_insert_test();
  test_suite_exists();
  test_test_exists();
  test_register_suite();

  test_cunit_end_tests();
}

#endif
