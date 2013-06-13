
/* Time-stamp: <2007-11-14 22:37:10 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include "mpiunit.h"
#include "mpiut_base.h"
#include "testdb.h"
#include "util.h"
#include "testrun.h"
#include "mpiut_basic.h"

static CU_pSuite suite;
static int myrank;

void
mpiut_setup()
{
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  CU_initialize_registry();
  suite = NULL;
  suite = CU_add_suite("", NULL, NULL);
  
  return;
}

void
mpiut_register(const char* name, CU_TestFunc test)
{
  CU_add_test(suite, name, test);
}

void
mpiut_run()
{
  int myrank;

  CU_basic_set_mode(CU_BRM_SILENT);
  CU_basic_run_tests();
  /* CU_basic_show_failures(CU_get_failure_list()); */
  CU_basic_exit();

  PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  fprintf(stdout, "Generate proclog file...rank%3d\n", myrank);
}

void
mpiut_show_result()
{
}

int
mpiut_rank()
{
  return myrank;
}
