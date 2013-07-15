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

void test_mpispec_setup(void)
{
  int argc = 3;
  char **argv;
  argv = (char *)malloc(10);
  argv[0] = (char *)malloc(10);
  argv[1] = (char *)malloc(10);
  argv[2] = (char *)malloc(10);
  argv[0] = "mpirun";
  argv[1] = "-np";
  argv[2] = "1";
  MPI_Init(&argc, &argv);
  free(argv);
}

void test_mpiut_rank(void)
{
  TEST_ASSERT_EQUAL(mpiut_rank(), 0);
}

void test_gettimeofday_sec(void)
{
  double t1, t2;
  t1 = gettimeofday_sec();
  usleep(1000000);
  t2 = gettimeofday_sec();
  TEST_ASSERT_EQUAL(t2 - t1, 1);
}
