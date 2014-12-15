#include <stdio.h>
#include "mpispec_error.h"
#include "mpispec_error_function.h"

MPISpecError MPISpec_Alloc_Error(void) {
    MPISpec_Error_Message("memory allocation failed.");
    return MPISPEC_ERROR;
}

MPISpecError MPISpec_Fopen_Error(void) {
    MPISpec_Error_Message("fopen failed.");
    return MPISPEC_ERROR;
}

MPISpecError MPISpec_Asprintf_Error(void) {
    MPISpec_Error_Message("asprintf failed.");
    return MPISPEC_ERROR;
}
