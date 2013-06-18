/*
 *  cspec_output_verbose.c   :  Verbose output with color (green for OK, red for fail and yellow for pending)
 *
 * See copyright notice in cspec.h
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <mpi.h>
#ifdef _WIN32
#  include <windows.h>
#endif
#include "cspec_output_verbose.h"

static CSpecOutputStruct verbose;
/* static CSpecOutputStructArray verbose_array[MAX_TEST_NUM]; */
static int verbose_array_index = 0;
int tab_num = 0;

/* private functions */
static void coloredPrintf(CSpec_Color color, const char* format, ...);
static void coloredFprintf(CSpec_Color color, const char* format, ...);
#ifdef _WIN32
static WORD getWindowsColorAttribute(CSpec_Color color);
#else  /* !_WIN32 */
static int getAnsiColorCode(CSpec_Color color);
#endif  /* _WIN32 */


void printTab(int n)
{
  int i;
  for (i = 0; i < n; i++)
    printf("  ");
}
void fprintTab(int n)
{
  int i;
  for (i = 0; i < n; i++)
    fprintf(__mpiut_result_file__, "  ");
}

void startDescribeFunVerbose( const char *descr)
{
  printf("%s\n", descr);
}
void f_startDescribeFunVerbose( const char *descr)
{
  fprintf(__mpiut_result_file__, "\n");
  fprintTab(++tab_num);
  fprintf(__mpiut_result_file__, "%s\n", descr);
}

void endDescribeFunVerbose( )
{
  tab_num--;
  printf("\n");
}
void f_endDescribeFunVerbose( )
{
  tab_num--;
  CSpecOutputStructArray cur_verbose = output_array[verbose_array_index];

  if(cur_verbose.Passed == cur_verbose.Total)
  {
    fprintf(__mpiut_result_file__, "\033[1;32mAll tests Passed (%d)\033[0m\n", cur_verbose.Total);
  }
  else
  {
    fprintf(__mpiut_result_file__, "\033[1;31mFailed\033[0m %d tests out of %d\n", cur_verbose.Total - cur_verbose.Passed, cur_verbose.Total);
  }
}

void startItFunVerbose( const char *descr)
{
  printTab(++tab_num);
  printf("- %s\n", descr);
}
void f_startItFunVerbose( const char *descr)
{
  fprintTab(++tab_num);
  fprintf(__mpiut_result_file__, "- %s\n", descr);
}

void endItFunVerbose( )
{
  printf("\n");
}
void f_endItFunVerbose( )
{
  tab_num--;
  /* fprintf(__mpiut_result_file__, "\n"); */
}

void endFunVerbose( )
{
  tab_num--;
  printf("\n");
}
void f_endFunVerbose( )
{
  tab_num--;
  fprintf(__mpiut_result_file__, "\n");
}

void startContextFunVerbose( const char *descr)
{
  printTab(++tab_num);
  printf("- %s\n", descr);
}
void f_startContextFunVerbose( const char *descr)
{
  fprintTab(++tab_num);
  fprintf(__mpiut_result_file__, "- %s\n", descr);
}

void endContextFunVerbose( const char *descr)
{
  tab_num--;
  printf("\n");
}
void f_endContextFunVerbose( const char *descr)
{
  tab_num--;
  /* fprintf(__mpiut_result_file__, "\n"); */
}

void evalFunVerbose(const char*filename, int line_number, const char*assertion, int assertionResult)
{
  printTab(tab_num + 1);
  if(assertionResult)
  {
    coloredPrintf(CSPEC_COLOR_GREEN,
        "OK: %s\n", assertion, filename, line_number);
  }
  else
  {
    coloredPrintf(CSPEC_COLOR_RED,
        "Failed: %s in file %s at line %d\n", assertion, filename, line_number);
  }
}
void f_evalFunVerbose(const char*filename, int line_number, const char*assertion, int assertionResult)
{
  output_array[verbose_array_index].Total++;
  fprintTab(tab_num + 1);
  if(assertionResult)
  {
    output_array[verbose_array_index].Passed++;
    coloredFprintf(CSPEC_COLOR_GREEN,
        "OK: %s\n", assertion, filename, line_number);
  }
  else
  {
    coloredFprintf(CSPEC_COLOR_RED,
        "Failed: %s in file %s at line %d\n", assertion, filename, line_number);
  }
}

void pendingFunVerbose(const char* reason)
{
  coloredPrintf(CSPEC_COLOR_YELLOW, "       Pending: %s\n", reason);
}
void f_pendingFunVerbose(const char* reason)
{
  coloredFprintf(CSPEC_COLOR_YELLOW, "       Pending: %s\n", reason);
}

CSpecOutputStruct* f_CSpec_NewOutputVerbose()
{
  CSpec_InitOutput(&verbose);

  verbose.startDescribeFun = startDescribeFunVerbose;
  verbose.endDescribeFun   = endDescribeFunVerbose;
  verbose.startItFun       = startItFunVerbose;
  verbose.endItFun         = endItFunVerbose;
  verbose.startContextFun  = startContextFunVerbose;
  verbose.endContextFun    = endContextFunVerbose;
  verbose.endFun           = endFunVerbose;
  verbose.evalFun          = evalFunVerbose;
  verbose.pendingFun       = pendingFunVerbose;


  return &verbose;
}

CSpecOutputStruct* CSpec_NewOutputVerbose()
{
  CSpec_InitOutput(&verbose);
  CSpec_InitOutputArray(&output_array[verbose_array_index]);

  verbose.startDescribeFun = f_startDescribeFunVerbose;
  verbose.endDescribeFun   = f_endDescribeFunVerbose;
  verbose.startItFun       = f_startItFunVerbose;
  verbose.endItFun         = f_endItFunVerbose;
  verbose.startContextFun  = f_startContextFunVerbose;
  verbose.endContextFun    = f_endContextFunVerbose;
  verbose.endFun           = f_endFunVerbose;
  verbose.evalFun          = f_evalFunVerbose;
  verbose.pendingFun       = f_pendingFunVerbose;

  output_array[verbose_array_index].output = verbose;
  verbose_array_index++;

  return &verbose;
}

#ifdef _WIN32
  static WORD
getWindowsColorAttribute(CSpec_Color color)
{
  WORD color_attribute;


  switch(color)
  {
    case CSPEC_COLOR_RED:
      color_attribute = FOREGROUND_RED;
      break;
    case CSPEC_COLOR_GREEN:
      color_attribute = FOREGROUND_GREEN;
      break;
    case CSPEC_COLOR_YELLOW:
      color_attribute = FOREGROUND_GREEN | FOREGROUND_RED;
      break;
    default:
      color_attribute = 0;
      break;
  }

  return color_attribute;
}
#else  /* !_WIN32 */
  static int
getAnsiColorCode(CSpec_Color color)
{
  int color_code;


  switch(color)
  {
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
#endif  /* _WIN32 */

  static void
coloredPrintf(CSpec_Color color, const char* format, ...)
{
#ifdef _WIN32
  HANDLE console_handle;
  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  WORD default_color_attributes;
#endif  /* _WIN32 */
  va_list args;


  va_start(args, format);

#ifdef _WIN32

  console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(console_handle, &buffer_info);
  default_color_attributes = buffer_info.wAttributes;

  /* Set color */
  SetConsoleTextAttribute(console_handle,
      getWindowsColorAttribute(color) |
      FOREGROUND_INTENSITY);

  /* Print Text */
  vprintf(format, args);

  /* Reset color */
  SetConsoleTextAttribute(console_handle,
      default_color_attributes);

#else  /* !_WIN32 */

  /* Set color */
  printf("\033[0;%dm", getAnsiColorCode(color));

  /* Print Text */
  vprintf(format, args);

  /* Reset color */
  printf("\033[m");

#endif  /* _WIN32 */

  va_end(args);
  return;
}

  static void
coloredFprintf(CSpec_Color color, const char* format, ...)
{
  va_list args;

  va_start(args, format);

  /* Set color */
  fprintf(__mpiut_result_file__, "\033[0;%dm", getAnsiColorCode(color));

  /* Print Text */
  vfprintf(__mpiut_result_file__, format, args);

  /* Reset color */
  fprintf(__mpiut_result_file__, "\033[m");

  va_end(args);
  return;
}
