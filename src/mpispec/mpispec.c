
/* Time-stamp: <2007-11-24 22:04:02 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

#include "mpispec_basic.h"

#define BAR "="

double gettimeofday_sec(void);
double test_start_time;

void
mpispec_setup()
{
  int myrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if(myrank == 0)
    test_start_time = gettimeofday_sec();

  CU_basic_set_mode(CU_BRM_VERBOSE);

  mpispec_make_result_file(&myrank);
}

void
mpispec_show_result()
{
  int myrank, n_procs, i;
  unsigned int local_specs, total_specs;
  unsigned int local_successspecs, total_successspecs;
  unsigned int local_failspecs, total_failspecs;
  double s_rate;
  char result_filename[32], ch;
  FILE *fp;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

  local_specs        = mpispec_get_number_of_specs();
  local_successspecs = mpispec_get_number_of_successes();
  local_failspecs    = mpispec_get_number_of_failures();

  PMPI_Reduce(&local_specs, &total_specs, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);
  PMPI_Reduce(&local_successspecs, &total_successspecs, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);
  PMPI_Reduce(&local_failspecs, &total_failspecs, 1, MPI_UNSIGNED,
              MPI_SUM, 0, MPI_COMM_WORLD);

  if(myrank == 0) {
    for(i = 0; i < n_procs; i++) {
      sprintf(result_filename, "rank%d.result", i);
      if(NULL == (fp = fopen(result_filename, "r"))) {
        fprintf(stderr, "Can't open result files");
        exit(-1);
      }
      while ((ch = fgetc(fp)) != EOF) {
        fputc(ch, stdout);
      }
      fclose(fp);
      remove(result_filename);
    }
    if(total_specs == 0) {
      s_rate = 100;
    }
    else {
      s_rate = (double)(total_specs - total_failspecs) /
        (double)(total_specs) * 100;
    }

    fprintf(stdout, "\n[%d Process Results]\n", n_procs);
    for(i = 1; i <= 50; i++){
      if(i < s_rate / 2)
        fprintf(stdout, "\033[1;32m%s\033[0m", BAR);
      else
        fprintf(stdout, "\033[1;31m%s\033[0m", BAR);
    }
    fprintf(stdout, "[%3.0lf%%]\n", s_rate);
    fprintf(stdout, "\nRun Time: %f sec\n", gettimeofday_sec() - test_start_time);
  }
}

int
mpiut_rank()
{
  int myrank;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  return myrank;
}

  double
gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}
