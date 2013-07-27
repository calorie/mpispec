/*
 *  cspec_private.c  :   Private functions
 *
 * See copyright notice in cspec.h
 *
 */

#include <stdio.h>
#include "mpispec_private.h"
#include "mpispec_output.h"

#define MAX_ARRAY_SIZE 64

static CSpecOutputStruct* CSpec_output = 0;
void ( *before_array[MAX_ARRAY_SIZE] )();

void
MPISpec_run_before();
void
MPISpec_init_all();

int
CSpec_StartDescribe(const char *descr)
{
  if( CSpec_output->startDescribeFun != NULL)
    CSpec_output->startDescribeFun(descr);

  return 0;
}

void
CSpec_EndDescribe()
{
  if( CSpec_output->endDescribeFun != NULL)
    CSpec_output->endDescribeFun();
}

int
CSpec_StartIt(const char *descr)
{
  MPISpec_run_before();
  if(CSpec_output->startItFun != NULL)
    CSpec_output->startItFun(descr);

  return 0;
}

void
CSpec_EndIt()
{
  if(CSpec_output->endItFun != NULL)
    CSpec_output->endItFun();
}

void
CSpec_End()
{
  if(CSpec_output->endFun != NULL)
    CSpec_output->endFun();
}

int
CSpec_StartContext(const char *descr)
{
  if(CSpec_output->startContextFun != NULL)
    CSpec_output->startContextFun(descr);

  return 0;
}

void
CSpec_EndContext()
{
  if(CSpec_output->endContextFun != NULL)
    CSpec_output->endContextFun();
}

void
CSpec_Eval(const char*filename, int line_number, const char*assertion, int assertionResult)
{
  if(CSpec_output->evalFun != NULL)
    CSpec_output->evalFun(filename, line_number, assertion, assertionResult);

  if (!assertionResult)
    CSpec_output->failed++;
}

void
CSpec_Pending(const char* reason)
{
  if(CSpec_output->pendingFun != NULL)
    CSpec_output->pendingFun(reason);
}

void
CSpec_SetOutput(CSpecOutputStruct* output)
{
  CSpec_output = output;
}

void
MPISpec_init_all()
{
  int i;
  for(i=0; i<MAX_ARRAY_SIZE; i++)
  {
    before_array[i] = NULL;
  }
}

void
MPISpec_set_before( MPISpecTmpFunction fun )
{
  int i;
  for(i=0; i<MAX_ARRAY_SIZE; i++)
  {
    if(before_array[i] == fun)
      break;
    if(before_array[i] == NULL) {
      before_array[i] = fun;
      break;
    }
  }
}

void
MPISpec_run_before()
{
  int i;
  for(i=0; i<MAX_ARRAY_SIZE; i++)
  {
    if(before_array[i] != NULL)
      before_array[i]();
    else
      break;
  }
}
