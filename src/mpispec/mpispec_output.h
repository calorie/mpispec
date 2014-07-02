/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:02:00
 */

/*
 *  cspec_output.h   :  Definition for output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef CSPEC_OUTPUT_H
#define CSPEC_OUTPUT_H

#include <stdio.h>

extern FILE *__mpiut_result_file__;

typedef void (*CSpecOutputStartDefFun)();
typedef void (*CSpecOutputEndDefFun)();

typedef void (*CSpecOutputStartDescribeFun)(const char *descr);
typedef void (*CSpecOutputEndDescribeFun)();

typedef void (*CSpecOutputStartItFun)(const char *descr);
typedef void (*CSpecOutputEndItFun)();

typedef void (*CSpecOutputEndFun)();

typedef void (*CSpecOutputEvalFun)(const char *filename, int line_number, const char *assertion, int assertionResult);
typedef void (*CSpecOutputPendingFun)(const char *reason);

typedef struct
{
    CSpecOutputStartDefFun      startDefFun;
    CSpecOutputEndDefFun        endDefFun;

    CSpecOutputStartDescribeFun startDescribeFun;
    CSpecOutputEndDescribeFun   endDescribeFun;

    CSpecOutputStartItFun       startItFun;
    CSpecOutputEndItFun         endItFun;

    CSpecOutputEndFun           endFun;

    CSpecOutputEvalFun          evalFun;
    CSpecOutputPendingFun       pendingFun;

    int failed;
} CSpecOutputStruct;

typedef struct
{
    unsigned int Passed;
    unsigned int Total;
} MPISpecRunSummary;

typedef MPISpecRunSummary *MS_pRunSummary;

static MPISpecRunSummary mpi_run_summary;

MS_pRunSummary get_mpi_run_summary(void);
void CSpec_InitOutput(CSpecOutputStruct *output);
void CSpec_SetOutput(CSpecOutputStruct *output);

#endif
