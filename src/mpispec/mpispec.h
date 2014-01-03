
/* Time-stamp: <2007-11-14 22:36:36 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#ifndef MPISPEC_H
#define MPISPEC_H

#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include "mpispec_basic.h"
#include "mpispec_config.h"
#include "mpispec_private.h"
#include "mpispec_output.h"
#include "mpispec_output_verbose.h"
#include "mpispec_output_junit_xml.h"

void
mpispec_setup();

void
mpispec_show_result();

int
mpiut_rank();

double
gettimeofday_sec();

/*               */
/* Public macros */
/*               */

/* Runner macros */

typedef void ( * CSpecDescriptionFun )();
int CSpec_Run( CSpecDescriptionFun fun, CSpecOutputStruct* output );

#define MPISpec_Run(test) \
  void test();            \
  CSpec_Run( test, CSpec_NewOutputVerbose() );
#define MPISpec_jxRun(test)                        \
  void test();                                     \
  MPISpec_JUnitXmlFileOpen("output.xml", "utf-8"); \
  CSpec_Run( test, CSpec_NewOutputJUnitXml() );


/* Config macros */

#define mpispec_init              \
  int                             \
  main( int argc, char **argv ) { \
    MPI_Init( &argc, &argv );     \
    mpispec_setup();

#define mpispec_finalize         \
  MPISpec_JUnitXmlFileClose();   \
  mpispec_run_summary();         \
  CU_basic_exit();               \
  MPI_Barrier( MPI_COMM_WORLD ); \
  mpispec_show_result();         \
  MPI_Finalize();                \
  return EXIT_SUCCESS; }
#define mpispec_fin mpispec_finalize

#define mpispec_rank \
  mpiut_rank()


/* Structural macros */

#define rank(rank)         { MPISpec_StartRank();       \
                           if( mpispec_rank == rank ) {
#define end_rank           } MPISpec_EndRank(); }

#define exclude_rank(rank) { MPISpec_StartRank();       \
                           if( mpispec_rank != rank ) {
#define ex_rank(rank)      exclude_rank( rank )
#define end_exclude_rank   } MPISpec_EndRank(); }
#define end_ex_rank        end_exclude_rank

#define ranks(ranks)      \
  { MPISpec_StartRanks(); \
  if( MPISpec_ValidateRanks( ranks, sizeof(ranks)/sizeof(ranks[0]), mpispec_rank ) == 0 ) {
#define end_ranks            } MPISpec_EndRanks(); }

#define exclude_ranks(ranks) \
  { MPISpec_StartRanks();    \
  if( MPISpec_ValidateRanks( ranks, sizeof(ranks)/sizeof(ranks[0]), mpispec_rank ) != 0 ) {
#define ex_ranks(ranks)      exclude_ranks( ranks )
#define end_exclude_ranks    } MPISpec_EndRanks(); }
#define end_ex_ranks         end_exclude_ranks

#define mpispec_def(test) void test() { MPISpec_StartDef(); {
#define end_def           } MPISpec_EndDef(); }

#define describe(caption) { CSpec_StartDescribe( caption ); {
#define end_describe      } CSpec_EndDescribe(); }

#define context(caption)  describe(caption)
#define end_context       end_describe

#define it(caption)       { CSpec_StartIt( caption ); {
#define end_it            } CSpec_EndIt(); }

/* gcc only */
#ifndef __clang__

#define before_each(foo)                     \
  auto void before_each_ ## foo (void);      \
  MPISpec_set_before( before_each_ ## foo ); \
  void before_each_ ## foo () {              \
  MPISpec_StartBefore(); {
#define end_before         } MPISpec_EndBefore(); }

#define after_each(foo)                    \
  auto void after_each_ ## foo (void);     \
  MPISpec_set_after( after_each_ ## foo ); \
  void after_each_ ## foo () {             \
  MPISpec_StartAfter(); {
#define end_after       } MPISpec_EndAfter(); }

#endif

#define end               } CSpec_End(); }


/* Expectation macros */

#define should_be_true(x)                       CSPEC_EVAL( (x) )
#define should_equal(x, y)                      CSPEC_EVAL( (x) == (y) )
#define should_equal_double(x, y, delta)        CSPEC_EVAL( cspec_fabs( (x) - (y) ) <= delta )
#define should_match(x, y)                      CSPEC_EVAL( cspec_strcmp(x, y) == 0 )
#define should_be_null(x)                       CSPEC_EVAL( (x) == 0 )

#define should_be_false(x)                      CSPEC_EVAL( !(x) )
#define should_not_equal(x, y)                  CSPEC_EVAL( (x) != (y) )
#define should_not_equal_double(x, y, delta)    CSPEC_EVAL( cspec_fabs( (x) - (y) ) > delta )
#define should_not_match(x, y)                  CSPEC_EVAL( cspec_strcmp(x, y) != 0 )
#define should_not_be_null(x)                   CSPEC_EVAL( (x) != 0 )

#define should_pending(reason)                  CSPEC_PENDING(reason)


/* Alias Expectation macros */

#define expect_to_be_true(x)                    should_be_true(x)
#define expect_to_equal(x, y)                   should_equal(x, y)
#define expect_to_equal_double(x, y, delta)     should_equal_double(x, y, delta)
#define expect_to_match(x, y)                   should_match(x, y)
#define expect_to_be_null(x)                    should_be_null(x)

#define expect_to_be_false(x)                   should_be_false(x)
#define expect_not_to_equal(x, y)               should_not_equal(x, y)
#define expect_not_to_equal_double(x, y, delta) should_not_equal_double(x, y, delta)
#define expect_not_to_match(x, y)               should_not_match(x, y)
#define expect_not_to_be_null(x)                should_not_be_null(x)

#define expect_to_pending(reason)               should_pending(reason)


/* MPI Expectation macros */

#define should_send_recv(fun, from, to, tag, timeout)        CSPEC_EVAL ( mpispec_send_recv(fun, from, to, tag, timeout) == 0 )
#define expect_to_send_recv(fun, from, to, tag, timeout)     should_send_recv(fun, from, to, tag, timeout)

#define should_not_send_recv(fun, from, to, tag, timeout)    CSPEC_EVAL ( mpispec_send_recv(fun, from, to, tag, timeout) != 0 )
#define expect_not_to_send_recv(fun, from, to, tag, timeout) should_not_send_recv(fun, from, to, tag, timeout)

#endif
