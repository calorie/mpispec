#include "unity.h"
#include <mpi.h>
#include "mpispec_config.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_cspec_strcmp(void)
{
  TEST_ASSERT_EQUAL(cspec_strcmp("abcdefg", "abcdefg"), 0);
  TEST_ASSERT_NOT_EQUAL(cspec_strcmp("abcdefg", "gfedcba"), 0);
}

void test_cspec_fabs(void)
{
  int r = rand();
  TEST_ASSERT_EQUAL(cspec_fabs(0), 0);
  TEST_ASSERT_EQUAL(cspec_fabs(r), r);
  TEST_ASSERT_EQUAL(cspec_fabs(-1*r), r);
  TEST_ASSERT_NOT_EQUAL(cspec_fabs(-1*r), -1*r);
}
