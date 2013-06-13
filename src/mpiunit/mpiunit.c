
/* Time-stamp: <2007-11-24 22:04:02 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mpiut_base.h"
#include "testdb.h"
#include "util.h"
#include "testrun.h"
#include "mpiut_basic.h"

#define BAR "============================================="

static CU_pSuite suite;

void
mpiut_setup()
{
  int myrank;
  char suitename[32];

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  sprintf(suitename, "rank%3d", myrank);
  CU_initialize_registry();
  suite = NULL;
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
  /* CU_basic_show_failures(CU_get_failure_list()); */
  CU_basic_exit();
}

void
mpiut_show_result()
{
  CU_pRunSummary pRunSummary;
  int myrank, n_procs;
  unsigned int local_tests, local_failtests;
  unsigned int local_asserts, local_failasserts;
  unsigned int total_tests, total_failtests;
  unsigned int total_asserts, total_failasserts;
  double s_rate;
  char result_filename[32];
  FILE *fp;
  char ch;
  int i;

  pRunSummary = CU_get_run_summary();

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

  local_tests = CU_get_number_of_tests_run();
  local_failtests = CU_get_number_of_tests_failed();
  local_asserts = CU_get_number_of_asserts();
  local_failasserts = CU_get_number_of_failures();

  PMPI_Reduce(&local_tests, &total_tests, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);
  PMPI_Reduce(&local_failtests, &total_failtests, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);
  PMPI_Reduce(&local_asserts, &total_asserts, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);
  PMPI_Reduce(&local_failasserts, &total_failasserts, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);

  if(myrank == 0) {
    for(i = 0; i < n_procs; i++) {
      /* printf("\n\n"); */
      sprintf(result_filename, "rank%d.result", i);
      if(NULL == (fp = fopen(result_filename, "r"))) {
        fprintf(stderr, "Can't open result files");
        exit(-1);
      }
      while(!feof(fp)) {
        ch = fgetc(fp);
        fputc(ch, stdout);
      }
      fclose(fp);
      remove(result_filename);
    }

    if(total_asserts == 0) {
      s_rate = 1.0;
    }
    else {
      s_rate = (double)(total_asserts - total_failasserts) /
        (double)(total_asserts);
    }

    fprintf(stdout, "\n[%d Process Results]\n", n_procs);
    if(total_failasserts == 0) {
      fprintf(stdout, "\033[1;32m%s\033[0m", BAR);
    }
    else {
      fprintf(stdout, "\033[1;31m%s\033[0m", BAR);
    }
    fprintf(stdout, "[%3.0lf\%]\n", 100 * s_rate);
  }
}

int
mpiut_rank()
{
  int myrank;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  return myrank;
}
