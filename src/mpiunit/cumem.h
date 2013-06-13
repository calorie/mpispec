
/* Time-stamp: <2007-11-14 22:36:23 shinya> */

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
 *  Contains Memory Related Defines to use internal routines to detect Memory Leak
 *  in Debug Versions
 *
 *  18/Jun/2002   Memory Debug Functions. (AK)
 *
 *  17-Jul-2004   New interface for global function names. (JDS)
 *
 *  05-Sep-2004   Added internal test interface. (JDS)
 */

/** @file
 *  Memory management functions (user interface).
 *  Two versions of memory allocation/deallocation are available.
 *  If compiled with MEMTRACE defined, CUnit keeps track of all
 *  system allocations & deallocations.  The memory record can
 *  then be reported using CU_CREATE_MEMORY_REPORT.  Otherwise,
 *  standard system memory allocation is used without tracing.
 */
/** @addtogroup Framework
 * @{
 */

#ifndef CUNIT_MYMEM_H_SEEN
#define CUNIT_MYMEM_H_SEEN

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MEMTRACE
  void*
  CU_calloc(size_t nmemb, size_t size, unsigned int uiLine,
            const char* szFileName);

  void*
  CU_malloc(size_t size, unsigned int uiLine, const char* szFileName);

  void
  CU_free(void *ptr, unsigned int uiLine, const char* szFileName);

  void*
  CU_realloc(void *ptr, size_t size, unsigned int uiLine,
             const char* szFileName);

  CU_EXPORT void
  CU_dump_memory_usage(const char*);

  #define CU_CALLOC(x, y)         CU_calloc((x), (y), __LINE__, __FILE__)
  #define CU_MALLOC(x)            CU_malloc((x), __LINE__, __FILE__)
  #define CU_FREE(x)              CU_free((x), __LINE__, __FILE__)
  #define CU_REALLOC(x, y)        CU_realloc((x), (y), __LINE__, __FILE__)
  #define CU_CREATE_MEMORY_REPORT(x) CU_dump_memory_usage((x))
  #define CU_DUMP_MEMORY_USAGE(x) CU_dump_memory_usage((x))

#else

  #define CU_CALLOC(x, y)         calloc((x), (y))
  #define CU_MALLOC(x)            malloc((x))
  #define CU_FREE(x)              free((x))
  #define CU_REALLOC(x, y)        realloc((x), (y))
  #define CU_CREATE_MEMORY_REPORT(x)
  #define CU_DUMP_MEMORY_USAGE(x)
#endif

#ifdef CUNIT_BUILD_TESTS
void
test_cunit_deactivate_malloc(void);

void
test_cunit_activate_malloc(void);

unsigned int
test_cunit_get_n_memevents(void* pLocation);

unsigned int 
test_cunit_get_n_allocations(void* pLocation);

unsigned int 
test_cunit_get_n_deallocations(void* pLocation);

void 
test_cunit_MyMem(void);
#endif 

#ifdef __cplusplus
}
#endif

#endif 
