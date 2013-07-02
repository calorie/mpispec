#include "unity.h"
#include <mpi.h>
#include "mpispec.h"

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
