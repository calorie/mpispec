/*
 *  cspec_private.c  :   Private functions
 *
 * See copyright notice in cspec.h
 *
 */

#include <stdio.h>
#include "mpispec_private.h"
#include "mpispec_output.h"

static CSpecOutputStruct* CSpec_output = 0;

#define MAX_ARRAY_SIZE 64
#define MAX_NEST_NUM   16
void ( *before_array[MAX_NEST_NUM][MAX_ARRAY_SIZE] )();
unsigned int nest_num = 0;

void
MPISpec_init_all();

void
MPISpec_remove_function();

void
MPISpec_run_before();

int
CSpec_StartDescribe( const char *descr )
{
  nest_num++;
  if( CSpec_output->startDescribeFun != NULL )
    CSpec_output->startDescribeFun( descr );

  return 0;
}

void
CSpec_EndDescribe()
{
  MPISpec_remove_function();

  if( CSpec_output->endDescribeFun != NULL )
    CSpec_output->endDescribeFun();
}

int
CSpec_StartIt( const char *descr )
{
  MPISpec_run_before();

  if( CSpec_output->startItFun != NULL )
    CSpec_output->startItFun( descr );

  return 0;
}

void
CSpec_EndIt()
{
  if( CSpec_output->endItFun != NULL )
    CSpec_output->endItFun();

  MPISpec_run_before();
}

void
CSpec_End()
{
  if( CSpec_output->endFun != NULL )
    CSpec_output->endFun();
}

void
CSpec_Eval( const char*filename, int line_number,
            const char*assertion, int assertionResult )
{
  if( CSpec_output->evalFun != NULL )
    CSpec_output->evalFun( filename, line_number, assertion, assertionResult );

  if ( !assertionResult )
    CSpec_output->failed++;
}

void
CSpec_Pending( const char* reason )
{
  if( CSpec_output->pendingFun != NULL )
    CSpec_output->pendingFun( reason );
}

void
CSpec_SetOutput( CSpecOutputStruct* output )
{
  CSpec_output = output;
}

void
MPISpec_init_all()
{
  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ ) {
    before_array[nest_num][i] = NULL;
  }
}

void
MPISpec_set_before( MPISpecTmpFunction fun )
{

  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ ) {
    if( before_array[nest_num][i] == fun )
      break;
    if( before_array[nest_num][i] == NULL ) {
      before_array[nest_num][i] = fun;
      break;
    }
  }
}

void
MPISpec_remove_function()
{
  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ ) {
    before_array[nest_num][i] = NULL;
  }
  nest_num--;
}

void
MPISpec_run_before()
{
  int i;
  for( i=0; i < MAX_ARRAY_SIZE; i++ ) {
    if( before_array[nest_num][i] != NULL )
      before_array[nest_num][i]();
    else
      break;
  }
}
