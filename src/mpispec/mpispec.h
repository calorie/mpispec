
/* Time-stamp: <2007-11-14 22:36:36 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#ifndef MPISPEC_H
#define MPISPEC_H

#include <mpi.h>
#include <stdlib.h>
#include "mpispec_basic.h"
#include "cspec.h"

void
mpispec_setup();

void
mpispec_show_result();

int
mpiut_rank();

double
gettimeofday_sec();

#define mpispec_init              \
  int                             \
  main( int argc, char **argv ) { \
    MPI_Init( &argc, &argv );     \
    mpispec_setup();

#define mpispec_prototype(test) \
  void test();

#define mpispec_def(test) \
  void test() { {

#define end_def \
  } }

#define mpispec_register(name, test) \
  mpiut_register( name, test );

#define mpispec_finalize         \
  mpispec_run_summary();         \
  CU_basic_exit();               \
  MPI_Barrier( MPI_COMM_WORLD ); \
  mpispec_show_result();         \
  MPI_Finalize();                \
  return EXIT_SUCCESS; }

#define mpispec_rank \
  mpiut_rank()

#endif
