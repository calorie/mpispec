
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
#include "mpispec_basic.h"

#define BAR "============================================="

void
mpispec_setup()
{
  CU_basic_set_mode(CU_BRM_VERBOSE);
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
  }
}

int
mpiut_rank()
{
  int myrank;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  return myrank;
}
