/*
 *  cspec_output_verbose.c   :  Verbose output with color (green for OK, red for fail and yellow for pending)
 *
 * See copyright notice in cspec.h
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include "mpispec_output_verbose.h"

static CSpecOutputStruct verbose;
int tab_num = 0;
MS_pRunSummary verbose_summary;

/* private functions */
void
fprintTab( int n );

static void
coloredFprintf( CSpec_Color color, const char* format, ... );

static int
getAnsiColorCode( CSpec_Color color );


void
startDefFunVerbose()
{
  verbose_summary = get_mpi_run_summary();
}

void
endDefFunVerbose()
{
}

void
startDescribeFunVerbose( const char *descr )
{
  fprintf( __mpiut_result_file__, "\n" );
  fprintTab( ++tab_num );
  fprintf( __mpiut_result_file__, "%s\n", descr );
}

void
endDescribeFunVerbose()
{
  tab_num--;
}

void
startItFunVerbose( const char *descr )
{
  fprintTab( ++tab_num );
  fprintf( __mpiut_result_file__, "- %s\n", descr );
}

void
endItFunVerbose()
{
  tab_num--;
}

void
endFunVerbose()
{
  tab_num--;
}

void
evalFunVerbose( const char*filename, int line_number,
                const char*assertion, int assertionResult )
{
  fprintTab( tab_num + 1 );

  verbose_summary->Total++;

  if( assertionResult ) {
    verbose_summary->Passed++;
    coloredFprintf( CSPEC_COLOR_GREEN,
        "OK: %s\n", assertion, filename, line_number );
  } else {
    coloredFprintf( CSPEC_COLOR_RED,
        "Failed: %s in file %s at line %d\n",
        assertion, filename, line_number );
  }
}

void
pendingFunVerbose(const char* reason)
{
  coloredFprintf( CSPEC_COLOR_YELLOW, "       Pending: %s\n", reason );
}

CSpecOutputStruct*
CSpec_NewOutputVerbose()
{
  CSpec_InitOutput( &verbose );

  verbose.startDefFun      = startDefFunVerbose;
  verbose.endDefFun        = endDefFunVerbose;
  verbose.startDescribeFun = startDescribeFunVerbose;
  verbose.endDescribeFun   = endDescribeFunVerbose;
  verbose.startItFun       = startItFunVerbose;
  verbose.endItFun         = endItFunVerbose;
  verbose.endFun           = endFunVerbose;
  verbose.evalFun          = evalFunVerbose;
  verbose.pendingFun       = pendingFunVerbose;

  return &verbose;
}

static int
getAnsiColorCode( CSpec_Color color )
{
  int color_code;

  switch( color ) {
    case CSPEC_COLOR_RED:
      color_code = 31;
      break;
    case CSPEC_COLOR_GREEN:
      color_code = 32;
      break;
    case CSPEC_COLOR_YELLOW:
      color_code = 33;
      break;
    default:
      color_code = 30;
      break;
  }

  return color_code;
}

void
fprintTab( int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    fprintf( __mpiut_result_file__, "  " );
}

static void
coloredFprintf( CSpec_Color color, const char* format, ... )
{
  va_list args;

  va_start( args, format );

  /* Set color */
  fprintf( __mpiut_result_file__, "\033[0;%dm", getAnsiColorCode( color ) );

  /* Print Text */
  vfprintf( __mpiut_result_file__, format, args );

  /* Reset color */
  fprintf( __mpiut_result_file__, "\033[m" );

  va_end( args );
  return;
}
