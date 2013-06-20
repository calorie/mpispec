
/* Time-stamp: <2007-11-14 22:36:36 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#ifndef MPIUT_H
#define MPIUT_H

#include <mpi.h>
#include <stdlib.h>
#include "mpiut_basic.h"
#include "mpiut_assertion.h"

void
mpiut_setup();
void
mpispec_setup();

void
mpiut_register(const char* name, CU_TestFunc test);

void
mpiut_run();

void
mpiut_show_result();
void
mpispec_show_result();

int
mpiut_rank();

#define MPIUT_INIT              \
  int                           \
  main(int argc, char **argv) { \
    MPI_Init(&argc, &argv);     \
    mpiut_setup()

#define mpispec_init            \
  int                           \
  main(int argc, char **argv) { \
    MPI_Init(&argc, &argv);     \
    mpispec_setup();

#define MPIUT_PROTOTYPE(test) \
  void test()

#define mpispec_prototype(test) \
  void test();

#define MPIUT_DEF(test) \
  void test()

#define mpispec_def(test) \
  void test() {{

#define end_def \
  }}

#define MPIUT_REGISTER(name, test) \
  mpiut_register(name, test)

#define mpispec_register(name, test) \
  mpiut_register(name, test);

#define MPIUT_FINALIZE         \
  mpiut_run();                 \
  MPI_Barrier(MPI_COMM_WORLD); \
  mpiut_show_result();         \
  MPI_Finalize();              \
  return EXIT_SUCCESS; }

#define mpispec_finalize       \
  mpispec_run_summary();       \
  CU_basic_exit();             \
  MPI_Barrier(MPI_COMM_WORLD); \
  mpiut_show_result();         \
  MPI_Finalize();              \
  return EXIT_SUCCESS; }

#define MPIUT_RANK \
  mpiut_rank()

#define mpispec_rank \
  mpiut_rank()

#endif
