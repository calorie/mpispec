/*
 *  cspec_output.c   :
 *
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#include <memory.h>
#include "mpispec_output.h"

void MPISpec_InitOutput(MPISpecOutputStruct *output) {
    memset(output, 0, sizeof(MPISpecOutputStruct));
}
