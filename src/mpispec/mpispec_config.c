/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 05:57:45
 */

/*
 *  cspec_config.c   :  This file depends on the capabilities of your hardware/platform
 *
 * In case your platform does not support double or provides another function to compute the absolute
 * value of a double, you can customize in this file.
 *
 * TODO: add a comparison function for cspec_double
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include <string.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>

#include "mpispec_config.h"

#define MPISPEC_TRUE               0
#define MPISPEC_FALSE              -1
#define MPISPEC_DEFAULT_TIMEOUT    2000000
#define MPISPEC_ONE_SEC            1000000
#define MPISPEC_ZERO_POINT_ONE_SEC 100000

int
cspec_strcmp ( const char * str1, const char * str2 )
{
  return strcmp( str1, str2 );
}

cspec_double
cspec_fabs( cspec_double arg )
{
  return fabs( arg );
}

mpispec_bool
mpispec_send_recv( MPISpecFun fun, int from, int to, int tag, double timeout )
{
  int test = MPISPEC_FALSE;
  int myrank;
  int diff = MPISPEC_ZERO_POINT_ONE_SEC;
  MPI_Status status;

  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  if( myrank == to ) {
    timeout *= MPISPEC_ONE_SEC;
    if( timeout < 1 )
      timeout = MPISPEC_DEFAULT_TIMEOUT;

    while( timeout > 0 ) {
      MPI_Iprobe( from, tag, MPI_COMM_WORLD, &test, &status );

      if( test == MPISPEC_TRUE )
        break;
      if( timeout < diff ) {
        usleep( timeout );
        break;
      }

      usleep( diff );

      timeout -= diff;
    }
  }

  if( myrank == from )
    fun();

  if( myrank == to )
    MPI_Send( &test, 1, MPI_INT, from, 0, MPI_COMM_WORLD );
  else if( myrank == from )
    MPI_Recv( &test, 1, MPI_INT, to, 0, MPI_COMM_WORLD, &status );

  return test;
}
