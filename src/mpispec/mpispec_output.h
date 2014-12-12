/*
 *  cspec_output.h   :  Definition for output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#ifndef MPISPEC_OUTPUT_H
#define MPISPEC_OUTPUT_H

#include <stdio.h>

#define MPISPEC_MAX_NEST_NUM 16
#define MPISPEC_MAX_XML_FILENAME_LEN 32
#define MPISPEC_TAB "  "

extern FILE *MPISPEC_GLOBAL_FP;

typedef void (*MPISpecDescFun)(void);

typedef void (*MPISpecOutputStartDefFun)(void);
typedef void (*MPISpecOutputEndDefFun)(void);

typedef void (*MPISpecOutputStartDescribeFun)(const char *descr);
typedef void (*MPISpecOutputEndDescribeFun)(void);

typedef void (*MPISpecOutputStartItFun)(const char *descr);
typedef void (*MPISpecOutputEndItFun)(void);

typedef void (*MPISpecOutputEndFun)(void);

typedef void (*MPISpecOutputEvalFun)(const char *filename, int line_number,
                                     const char *assertion,
                                     int assertionResult);
typedef void (*MPISpecOutputPendingFun)(const char *reason);

typedef struct {
    MPISpecOutputStartDefFun start_def_fun;
    MPISpecOutputEndDefFun end_def_fun;

    MPISpecOutputStartDescribeFun start_describe_fun;
    MPISpecOutputEndDescribeFun end_describe_fun;

    MPISpecOutputStartItFun start_it_fun;
    MPISpecOutputEndItFun end_it_fun;

    MPISpecOutputEndFun end_fun;

    MPISpecOutputEvalFun eval_fun;
    MPISpecOutputPendingFun pending_fun;

    int failed;
} MPISpecOutputStruct;

typedef struct {
    unsigned int Passed;
    unsigned int Total;
} MPISpecRunSummary;

typedef MPISpecRunSummary *pMPISpecRunSummary;

pMPISpecRunSummary MPISpec_Get_Run_Summary(void);
void MPISpec_InitOutput(MPISpecOutputStruct *output);

#endif
