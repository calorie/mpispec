/*
 *  cspec_output.h   :  Definition for output
 *
 * See copyright notice in cspec.h
 *
 */
#ifndef CSPEC_OUTPUT_H
#define CSPEC_OUTPUT_H

#include <stdio.h>
extern FILE *__mpiut_result_file__;
typedef void ( * CSpecOutputStartDescribeFun ) ( const char *descr);
typedef void ( * CSpecOutputEndDescribeFun ) ( );

typedef void ( * CSpecOutputStartItFun ) ( const char *descr);
typedef void ( * CSpecOutputEndItFun ) ( );
typedef void ( * CSpecOutputEndFun ) ( );

typedef void ( * CSpecOutputStartContextFun ) ( const char *descr);
typedef void ( * CSpecOutputEndContextFun ) ( );

typedef void ( * CSpecOutputEvalFun ) (const char*filename, int line_number, const char*assertion, int assertionResult);
typedef void ( * CSpecOutputPendingFun ) (const char* reason);

typedef struct
{

    CSpecOutputStartDescribeFun startDescribeFun;
    CSpecOutputEndDescribeFun   endDescribeFun;

    CSpecOutputStartItFun       startItFun;
    CSpecOutputEndItFun         endItFun;
    CSpecOutputEndFun           endFun;

    CSpecOutputStartContextFun  startContextFun;
    CSpecOutputEndContextFun    endContextFun;

    CSpecOutputEvalFun          evalFun;
    CSpecOutputPendingFun       pendingFun;

    int failed;

} CSpecOutputStruct;

void CSpec_InitOutput(CSpecOutputStruct* output);

void CSpec_SetOutput(CSpecOutputStruct* output);

#endif
