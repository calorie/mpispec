#ifndef MPISPEC_ERROR_FUNCTION_H
#define MPISPEC_ERROR_FUNCTION_H

#include "mpispec_error.h"

MPISpecError MPISpec_Alloc_Error(void);
MPISpecError MPISpec_Fopen_Error(void);
MPISpecError MPISpec_Asprintf_Error(void);

#endif
