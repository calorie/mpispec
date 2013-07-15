
/* Time-stamp: <2007-11-14 22:37:00 shinya> */

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
 *  Copyright (C) 2004,2005,2006  Jerry St.Clair
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
 *  Interface for simple test runner.
 *
 *  11-Aug-2004   Initial implementation of basic test runner interface. (JDS)
 */

/** @file
 * Basic interface with output to stdout.
 */
/** @addtogroup Basic
 * @{
 */

#ifndef MPISPEC_BASIC_H_SEEN
#define MPISPEC_BASIC_H_SEEN

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  CU_BRM_NORMAL  = 0,
  CU_BRM_SILENT  = 1,
  CU_BRM_VERBOSE = 2
} CU_BasicRunMode;

void
CU_basic_set_mode(CU_BasicRunMode mode);

CU_BasicRunMode
CU_basic_get_mode(void);

void
CU_basic_exit();

void
mpispec_make_result_file(int *myrank);

void
mpispec_run_summary(void);

unsigned int
mpispec_get_number_of_specs(void);
unsigned int
mpispec_get_number_of_successes(void);
unsigned int
mpispec_get_number_of_failures(void);

#ifdef __cplusplus
}
#endif

#endif
