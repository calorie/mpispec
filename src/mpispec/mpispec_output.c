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

void MPISpec_InitOutput(MPISpecOutputStruct *output) {
    memset(output, 0, sizeof(MPISpecOutputStruct));
}

pMPISpecRunSummary MPISpec_Get_Run_Summary(void) { return &mpi_run_summary; }
