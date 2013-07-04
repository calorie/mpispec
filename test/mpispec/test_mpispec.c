#include "unity.h"
#include <mpi.h>
#include "mpispec.h"
#include <unistd.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mpiut_rank(void)
{
  MPI_Init(NULL, NULL);
  TEST_ASSERT_EQUAL(mpiut_rank(), 0);
}

void test_gettimeofday_sec(void)
{
  double t1, t2;
  t1 = gettimeofday_sec();
  usleep(1000000);
  t2 = gettimeofday_sec();
  TEST_ASSERT_EQUAL(t2 - t1, 1.0);
}
