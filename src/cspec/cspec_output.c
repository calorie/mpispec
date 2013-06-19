/*
 *  cspec_output.c   :
 *
 *
 * See copyright notice in cspec.h
 *
 */

#include "cspec.h"
#include <stdio.h>
#include <memory.h>

void CSpec_InitOutput( CSpecOutputStruct* output )
{
  memset(output, 0, sizeof(CSpecOutputStruct) );
}

MS_pRunSummary get_mpi_run_summary(void)
{
  return &mpi_run_summary;
}
