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
#define MAX_RANKS_NUM  1024
void ( *before_array[MAX_NEST_NUM][MAX_ARRAY_SIZE] )();
void ( *after_array[MAX_NEST_NUM][MAX_ARRAY_SIZE] )();
void ( *end_fun_stack[MAX_NEST_NUM + 1] )();
static unsigned int nest_num = 0;

void
MPISpec_remove_before();
void
MPISpec_run_before();

void
MPISpec_remove_after();
void
MPISpec_run_after();

void
MPISpec_push_end_fun( MPISpecTmpFunction end_fun );

void
MPISpec_pop_end_fun();

int
MPISpec_StartRank()
{
  MPISpec_push_end_fun( MPISpec_EndRank );
  return 0;
}

void
MPISpec_EndRank()
{
}

int
MPISpec_StartRanks()
{
  MPISpec_push_end_fun( MPISpec_EndRanks );
  return 0;
}

void
MPISpec_EndRanks()
{
}

int
MPISpec_ValidateRanks( int ranks[], const int size, int myrank )
{
  int i, result = 1;

  for( i = 0; i < size; i++ ) {
    if( i >= MAX_RANKS_NUM ) {
      printf("\033[1;33mWARN : ranks size over max size.\033[0m\n");
      break;
    }
    if( ranks[i] == myrank ) {
      result = 0;
      break;
    }
  }
  return result;
}

int
MPISpec_StartDef()
{
  if( CSpec_output->startDefFun != NULL )
    CSpec_output->startDefFun();

  MPISpec_push_end_fun( MPISpec_EndDef );

  return 0;
}

void
MPISpec_EndDef()
{
  if( CSpec_output->endDefFun != NULL )
    CSpec_output->endDefFun();
}

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
  MPISpec_remove_after();
  nest_num--;

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
  MPISpec_run_after();

  if( CSpec_output->endItFun != NULL )
    CSpec_output->endItFun();
}

int
MPISpec_StartBefore()
{
  MPISpec_push_end_fun( MPISpec_EndBefore );
  return 0;
}

void
MPISpec_EndBefore()
{
}

int
MPISpec_StartAfter()
{
  MPISpec_push_end_fun( MPISpec_EndAfter );
  return 0;
}

void
MPISpec_EndAfter()
{
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
MPISpec_set_after( MPISpecTmpFunction fun )
{
  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ ) {
    if( after_array[nest_num][i] == fun )
      break;
    if( after_array[nest_num][i] == NULL ) {
      after_array[nest_num][i] = fun;
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
MPISpec_remove_after()
{
  int i;
  for( i = 0; i < MAX_ARRAY_SIZE; i++ )
    after_array[nest_num][i] = NULL;
}

void
MPISpec_run_after()
{
  int i, j;
  for( i = 1; i <= nest_num; i++ ) {
    for( j = 0; j < MAX_ARRAY_SIZE; j++ ) {
      if( after_array[i][j] != NULL )
        after_array[i][j]();
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
