/*
 *  cspec_output_xml.c   :  Xml output
 *
 * See copyright notice in cspec.h
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include "mpispec_output_xml.h"
#include "mpispec_consts.h"

static CSpecOutputStruct xml;
static FILE *outputXmlFile = NULL;
static int mpispec_tab_num = 0;

static void
fprintTab( int n );

void
MPISpec_XmlFileOpen(const char *filename, const char *encoding)
{
  if (outputXmlFile != NULL)
    return;

  time_t timeValue;
  char*  timeStr;
  char   xml_filename[MPISPEC_MAX_XML_FILENAME_LEN];
  int    myrank;
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  sprintf( xml_filename, "rank%d_%s", myrank, filename );
  outputXmlFile = fopen(xml_filename, "w");

  if (outputXmlFile == NULL)
    return;

  time(&timeValue);
  timeStr = ctime(&timeValue);
  timeStr[strlen(timeStr) - 1] = '\0';

  fprintf(outputXmlFile, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n", encoding);
  fprintf(outputXmlFile, "<?xml-stylesheet type=\"text/xsl\" href=\"CSpec-Run.xsl\" ?>\n");
  fprintf(outputXmlFile, "<BEHAVIOUR timestump=\"%s\">\n", timeStr);
  mpispec_tab_num++;
}

void
MPISpec_XmlFileClose(void)
{
  if (outputXmlFile == NULL)
    return;

  mpispec_tab_num--;
  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "</BEHAVIOUR>\n");

  fclose(outputXmlFile);
}


void
startDescribeFunXml(const char *descr)
{
  if (outputXmlFile == NULL)
    return;

  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "<DESCRIBE>\n");
  fprintTab( mpispec_tab_num + 1 );
  fprintf(outputXmlFile, "<DESCRIPTION><![CDATA[%s]]></DESCRIPTION>\n", descr);
  mpispec_tab_num++;
}

void
endDescribeFunXml(void)
{
  if (outputXmlFile == NULL)
    return;

  mpispec_tab_num--;
  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "</DESCRIBE>\n");
}

void
startItFunXml(const char *descr)
{
  if (outputXmlFile == NULL)
    return;

  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "<IT>\n");
  fprintTab( mpispec_tab_num + 1 );
  fprintf(outputXmlFile, "<DESCRIPTION><![CDATA[it %s]]></DESCRIPTION>\n", descr);
}

void
endItFunXml()
{
  if (outputXmlFile == NULL)
    return;

  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "</IT>\n");
}

void
evalFunXml(const char *filename, int line_number, const char *assertion, int assertionResult)
{
  if (outputXmlFile == NULL)
    return;

  mpispec_tab_num++;
  if (assertionResult) {
    fprintTab( mpispec_tab_num );
    fprintf(outputXmlFile, "<ASSERTION>\n");

    fprintTab( mpispec_tab_num + 1 );
    fprintf(outputXmlFile, "<RESULT>OK</RESULT>\n");
    fprintTab( mpispec_tab_num + 1 );
    fprintf(outputXmlFile, "<MESSAGE><![CDATA[%s]]></MESSAGE>\n", assertion);

    fprintTab( mpispec_tab_num );
    fprintf(outputXmlFile, "</ASSERTION>\n");
  } else {
    fprintTab( mpispec_tab_num );
    fprintf(outputXmlFile, "<ASSERTION>\n");

    fprintTab( mpispec_tab_num + 1 );
    fprintf(outputXmlFile, "<RESULT>Failure</RESULT>\n");
    fprintTab( mpispec_tab_num + 1 );
    fprintf(outputXmlFile, "<MESSAGE><![CDATA[%s in file %s at line %d]]></MESSAGE>\n", assertion, filename, line_number);

    fprintTab( mpispec_tab_num );
    fprintf(outputXmlFile, "</ASSERTION>\n");
  }
  mpispec_tab_num--;
}

void
pendingFunXml(const char* reason)
{
  if (outputXmlFile == NULL)
    return;

  mpispec_tab_num++;
  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "<ASSERTION>\n");

  fprintTab( mpispec_tab_num + 1 );
  fprintf(outputXmlFile, "<RESULT>Pending</RESULT>\n");
  fprintTab( mpispec_tab_num + 1 );
  fprintf(outputXmlFile, "<MESSAGE><![CDATA[%s]]></MESSAGE>\n", reason);

  fprintTab( mpispec_tab_num );
  fprintf(outputXmlFile, "</ASSERTION>\n");
}

CSpecOutputStruct*
CSpec_NewOutputXml()
{
  CSpec_InitOutput(&xml);

  xml.startDescribeFun = startDescribeFunXml;
  xml.endDescribeFun   = endDescribeFunXml;
  xml.startItFun       = startItFunXml;
  xml.endItFun         = endItFunXml;
  xml.evalFun          = evalFunXml;
  xml.pendingFun       = pendingFunXml;

  return &xml;
}

static void
fprintTab( int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    fprintf( outputXmlFile, MPISPEC_TAB );
}
