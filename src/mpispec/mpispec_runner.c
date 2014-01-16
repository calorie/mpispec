/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:07:18
 */

/*
 *  cspec_runner.c   :  The runner
 *
 * TODO: What if fun or output are NULL? Should we set the output for each run?
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include "mpispec.h"

int
CSpec_Run( CSpecDescriptionFun fun, CSpecOutputStruct* output )
{
  CSpec_SetOutput( output );
  fun();
  return output->failed;
}

