
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

#define BAR "============================================="

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
  int myrank, n_procs;
  double s_rate;
  char result_filename[32];
  FILE *fp;
  char ch;
  int i;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

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
    printf("\nRun Time: %f sec\n", gettimeofday_sec() - test_start_time);
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
