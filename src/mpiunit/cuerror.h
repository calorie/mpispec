
/* Time-stamp: <2007-11-14 22:36:16 shinya> */

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
 *  Copyright (C) 2001            Anil Kumar
 *  Copyright (C) 2004,2005,2006  Anil Kumar, Jerry St.Clair
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
 *  Contains CUnit error codes which can be used externally.
 *
 *  Aug 2001      Initial implementation.  (AK)
 *
 *  02/Oct/2001   Added proper Eror Codes. (AK)
 *
 *  13-Oct-2001   Added Error Codes for Duplicate TestGroup and Test. (AK)
 *
 *  03-Aug-2004   Converted error code macros to an enum, doxygen comments, moved
 *                error handing code here, changed file name from Errno.h, added
 *                error codes for file open errors, added error action selection. (JDS)
 *
 *  05-Sep-2004   Added internal test interface. (JDS)
 */

/** @file
 *  Error handling functions (user interface).
 *  CUnit uses a simple (and conventional) error handling strategy.
 *  Functions that can generate errors set (and usually return) an
 *  error code to indicate the run status.  The error code can be
 *  inspected using the CU_get_error() function.  A descriptive
 *  error message can be retrieved using CU_get_error_msg().
 */
/** @addtogroup Framework
 * @{
 */

#ifndef CUNIT_CUERROR_H_SEEN
#define CUNIT_CUERROR_H_SEEN

#include <errno.h>

typedef enum {
  CUE_SUCCESS           = 0,
  CUE_NOMEMORY          = 1,
  CUE_NOREGISTRY        = 10,
  CUE_REGISTRY_EXISTS   = 11,
  CUE_NOSUITE           = 20,
  CUE_NO_SUITENAME      = 21,
  CUE_SINIT_FAILED      = 22,
  CUE_SCLEAN_FAILED     = 23,
  CUE_DUP_SUITE         = 24,
  CUE_NOTEST            = 30, 
  CUE_NO_TESTNAME       = 31, 
  CUE_DUP_TEST          = 32, 
  CUE_TEST_NOT_IN_SUITE = 33, 
  CUE_FOPEN_FAILED      = 40,
  CUE_FCLOSE_FAILED     = 41,
  CUE_BAD_FILENAME      = 42,
  CUE_WRITE_ERROR       = 43 
} CU_ErrorCode;

typedef enum CU_ErrorAction {
  CUEA_IGNORE,
  CUEA_FAIL,  
  CUEA_ABORT  
} CU_ErrorAction;

#include "mpiut_base.h"

#ifdef __cplusplus
extern "C" {
#endif

CU_EXPORT CU_ErrorCode
CU_get_error(void);

CU_EXPORT const char*
CU_get_error_msg(void);

CU_EXPORT void
CU_set_error_action(CU_ErrorAction action);

CU_EXPORT CU_ErrorAction 
CU_get_error_action(void);

#ifdef CUNIT_BUILD_TESTS
void test_cunit_CUError(void);
#endif

void  
CU_set_error(CU_ErrorCode error);

#ifdef __cplusplus
}
#endif

#ifdef USE_DEPRECATED_CUNIT_NAMES
#define get_error() CU_get_error_msg()
#endif 

#endif 
