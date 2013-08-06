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
void ( *end_fun_stack[MAX_NEST_NUM + 1] )();
static unsigned int nest_num = 0;

void
MPISpec_remove_before();

void
MPISpec_run_before();

void
MPISpec_push_end_fun( MPISpecTmpFunction end_fun );

void
MPISpec_pop_end_fun();

int
CSpec_StartDescribe( const char *descr )
{
  nest_num++;

  if( CSpec_output->startDescribeFun != NULL )
    CSpec_output->startDescribeFun( descr );

  MPISpec_push_end_fun( CSpec_EndDescribe );

  return 0;
}

void
CSpec_EndDescribe()
{
  MPISpec_remove_before();

  if( CSpec_output->endDescribeFun != NULL )
    CSpec_output->endDescribeFun();
}

int
CSpec_StartIt( const char *descr )
{
  MPISpec_run_before();

  if( CSpec_output->startItFun != NULL )
    CSpec_output->startItFun( descr );

  MPISpec_push_end_fun( CSpec_EndIt );

  return 0;
}

void
CSpec_EndIt()
{
  if( CSpec_output->endItFun != NULL )
    CSpec_output->endItFun();
}

int
MPISpec_StartBefore()
{
  if( CSpec_output->startBeforeFun != NULL )
    CSpec_output->startBeforeFun();

  MPISpec_push_end_fun( MPISpec_EndBefore );
}

void
MPISpec_EndBefore()
{
  if( CSpec_output->endBeforeFun != NULL )
    CSpec_output->endBeforeFun();
}

void
CSpec_End()
{
  MPISpec_pop_end_fun();
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
MPISpec_remove_before()
{
  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ )
    before_array[nest_num][i] = NULL;
  nest_num--;
}

void
MPISpec_run_before()
{
  int i, j;
  for( i = 1; i <= nest_num; i++ ) {
    for( j = 0; j < MAX_ARRAY_SIZE; j++ ) {
      if( before_array[i][j] != NULL )
        before_array[i][j]();
      else
        break;
    }
  }
}

void
MPISpec_push_end_fun( MPISpecTmpFunction end_fun )
{
  int i;
  for( i = 0; i < MAX_NEST_NUM; i++ ) {
    if( end_fun_stack[i] == NULL ) {
      end_fun_stack[i] = end_fun;
      break;
    }
  }
}


void
MPISpec_pop_end_fun()
{
  int i;
  for( i = 1; i <= MAX_NEST_NUM; i++ )
    if( end_fun_stack[i] == NULL )
      break;

  if( end_fun_stack[i - 1] != NULL) {
    end_fun_stack[i - 1]();
    end_fun_stack[i - 1] = NULL;
  }
}
