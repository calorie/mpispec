/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:01:36
 */

/*
 *  cspec_output.c   :
 *
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include <memory.h>

#include "mpispec_output.h"

static MPISpecRunSummary mpi_run_summary;

void
MPISpec_InitOutput(MPISpecOutputStruct *output)
{
    memset(output, 0, sizeof(MPISpecOutputStruct));
}

pMPISpecRunSummary
get_mpi_run_summary(void)
{
    return &mpi_run_summary;
}
