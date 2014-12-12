/*
 *  cspec_runner.c   :  The runner
 *
 * TODO: What if fun or output are NULL? Should we set the output for each run?
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#include "mpispec_output.h"
#include "mpispec_private.h"
#include "mpispec_runner.h"

int MPISpec_Run(MPISpecDescFun fun, MPISpecOutputStruct* output) {
    MPISpec_SetOutput(output);
    fun();
    return output->failed;
}
