
/* Time-stamp: <2007-11-24 23:04:56 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

/*
 *  Original:
 *
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Implementation for basic test runner interface.
 *
 *  Created By  : Jerry St.Clair  (11-Aug-2004)
 *  Comment     : Initial implementation of basic test runner interface
 *  EMail       : jds2@users.sourceforge.net
 *
 *  Modified    : 8-Jan-2005 (JDS)
 *  Comment     : Fixed reporting bug (bug report cunit-Bugs-1093861).
 *  Email       : jds2@users.sourceforge.net
 *
 *  Modified    : 30-Apr-2005 (JDS)
 *  Comment     : Added notification of suite cleanup failure.
 *  Email       : jds2@users.sourceforge.net
 */

/** @file
 * Basic interface with output to stdout.
 */
/** @addtogroup Basic
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "mpispec_basic.h"
#include "mpispec_output.h"

FILE *__mpiut_result_file__;

static CU_BasicRunMode f_run_mode = CU_BRM_NORMAL;

void
CU_basic_set_mode(CU_BasicRunMode mode)
{
  f_run_mode = mode;
}

CU_BasicRunMode
CU_basic_get_mode(void)
{
  return f_run_mode;
}

void
CU_basic_exit()
{
  fclose(__mpiut_result_file__);
}

void
mpispec_make_result_file(int *myrank)
{
  char result_filename[32];

  MPI_Comm_rank(MPI_COMM_WORLD, myrank);
  sprintf(result_filename, "rank%d.result", *myrank);
  if(NULL == (__mpiut_result_file__ = fopen(result_filename, "a"))) {
    fprintf(stderr, "Can't open result files");
    exit(-1);
  };
  fprintf(__mpiut_result_file__, "\nrank  %d:", *myrank);
}

void
mpispec_run_summary(void)
{
  MS_pRunSummary summary = get_mpi_run_summary();

  fprintf(__mpiut_result_file__,"\n--Run Summary: Type      Total  Passed  Failed"
      "\n               tests  %8u%8u%8u\n",
      summary->Total,
      summary->Passed,
      summary->Total - summary->Passed);
}

unsigned int
mpispec_get_number_of_specs(void)
{
  return get_mpi_run_summary()->Total;
}
unsigned int
mpispec_get_number_of_successes(void)
{
  return get_mpi_run_summary()->Passed;
}
unsigned int
mpispec_get_number_of_failures(void)
{
  MS_pRunSummary summary = get_mpi_run_summary();
  return summary->Total - summary->Passed;
}
