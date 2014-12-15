#include <stdio.h>
#include "mpispec_error.h"
#include "mpispec_util.h"

#define ERRORS_MAX 16

static MPISpecError error = 0;
static MPISpecErrorFun errors[ERRORS_MAX];

MPISpecError MPISpec_Error(void) { return error; }

void MPISpec_Init_Errors(void) {
    int i;
    for (i = 0; i < ERRORS_MAX; i++) errors[i] = NULL;
}

MPISpecError MPISpec_Set_Error(void) {
    error = MPISPEC_ERROR;
    return error;
}

void MPISpec_Set_Error_Fun(MPISpecErrorFun fun) {
    int i;
    MPISpec_Set_Error();
    for (i = 0; i < ERRORS_MAX; i++) {
        if (errors[i] == NULL) {
            errors[i] = fun;
            break;
        }
    }
}

void MPISpec_Run_Errors(void) {
    if (!error) return;
    int i;
    for (i = 0; i < ERRORS_MAX; i++) {
        if (errors[i] == NULL) break;
        errors[i]();
    }
}

void MPISpec_Error_Message(char *msg) {
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "[rank %d] %s\n", MPISpec_Rank(), msg);
    fprintf(stderr, "\033[0;0m");
}
