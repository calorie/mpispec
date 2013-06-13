
/* Time-stamp: <2007-11-14 22:37:14 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include "mpiunit.h"
#include "mpiut_base.h"
#include "mpiut_basic.h"

#define PROGRESS_BAR_SIZE 45

static CU_pSuite suite;
static int myrank;

void
mpiut_setup()
{
  char suitename[32];
  
  suite = NULL;
  CU_initialize_registry();

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  sprintf(suitename, "rank%3d", myrank);
  
  suite = CU_add_suite(suitename, NULL, NULL);
}

void
mpiut_register(const char* name, CU_TestFunc test)
{
  CU_add_test(suite, name, test);
}

void
mpiut_run()
{
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n");
  /* CU_basic_show_failures(CU_get_failure_list()); */
  /* printf("\n\n"); */
}

void
mpiut_show_result()
{
  unsigned int n_asserts, n_successes;
  double s_rate;
  char green_bar[PROGRESS_BAR_SIZE + 1] = {0};
  char red_bar[PROGRESS_BAR_SIZE + 1] = {0};
  int i, g_count, r_count;

  n_asserts = CU_get_number_of_asserts();
  n_successes = CU_get_number_of_successes();
  if(n_asserts == 0) {
    s_rate = 1.0;
  }
  else {
    s_rate = (double)(n_successes) / (double)(n_asserts);
  }

  g_count = 0;
  r_count = 0;
  for(i = 0; i < PROGRESS_BAR_SIZE; i++) {
    if(PROGRESS_BAR_SIZE * s_rate >= i) {
      green_bar[g_count] = '=';
      g_count++;
    }
    else {
      red_bar[r_count] = '=';
      r_count++;
    }
  }

  fprintf(stdout, "\033[1;32m%s\033[0m", green_bar);
  fprintf(stdout, "\033[1;31m%s\033[0m", red_bar);
  fprintf(stdout, "[%d/%d][%3.0lf\%]\n\n",
          n_successes, n_asserts, 100 * s_rate);
}

int
mpiut_rank()
{
  return myrank;
}
