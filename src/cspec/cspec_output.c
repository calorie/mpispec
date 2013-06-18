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

void CSpec_InitOutputArray( CSpecOutputStructArray* output_array )
{
  memset(output_array, 0, sizeof(CSpecOutputStructArray) );
}
