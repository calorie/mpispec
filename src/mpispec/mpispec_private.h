/*
 *  cspec_private.h
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#ifndef MPISPEC_PRIVATE_H
#define MPISPEC_PRIVATE_H

#include "mpispec_output.h"

int MPISpec_StartRank(void);
void MPISpec_EndRank(void);

int MPISpec_StartRanks(void);
void MPISpec_EndRanks(void);

int MPISpec_ValidateRanks(int ranks[], const int size, int rank);

int MPISpec_StartDef(void);
void MPISpec_EndDef(void);

int MPISpec_StartDescribe(const char *descr);
void MPISpec_EndDescribe(void);

int MPISpec_StartIt(const char *descr);
void MPISpec_EndIt(void);

int MPISpec_StartBefore(void);
void MPISpec_EndBefore(void);

int MPISpec_StartAfter(void);
void MPISpec_EndAfter(void);

void MPISpec_End(void);

void MPISpec_Eval(const char *filename, int line_number, const char *assertion,
                  int assertion_result);

void MPISpec_Pending(const char *reason);

void MPISpec_SetOutput(MPISpecOutputStruct *output);

#define MPISPEC_EVAL(x) \
    { MPISpec_Eval(__FILE__, __LINE__, #x, (x)); }
#define MPISPEC_PENDING(reason) \
    { MPISpec_Pending(reason); }

typedef void (*MPISpecTmpFunction)(void);

void MPISpec_Set_Before(MPISpecTmpFunction fun);
void MPISpec_Set_After(MPISpecTmpFunction fun);

#endif
