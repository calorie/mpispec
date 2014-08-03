#ifndef MPISPEC_PRIVATE_OUTPUT_XML_H
#define MPISPEC_PRIVATE_OUTPUT_XML_H

#include "array.h"

typedef struct {
    const char *message;
    const char *type;
    const char *fname;
    int        line;
    const char *assertion_descr;
} MPISpecFailureStruct;

typedef struct {
    int     assert_num;
    int     pending_num;
    char    *descr;
    array_t *failures;
} MPISpecItOutputStruct;

typedef struct {
    char                  *descr;
    int                   it_output_num;
    MPISpecItOutputStruct *it_outputs;
} MPISpecDescOutputStruct;

#endif
