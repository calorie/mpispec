/*
 *  cspec_output_juni_xml.h   :  Declaration of the junit xml output
 *
 * See copyright notice in cspec.h
 *
 */

#ifndef MPISPEC_OUTPUT_JUNIT_XML_H
#define MPISPEC_OUTPUT_JUNIT_XML_H

#include "mpispec_output.h"

CSpecOutputStruct* CSpec_NewOutputJUnitXml();

void MPISpec_JUnitXmlFileOpen(const char *filename, const char *encoding);
void MPISpec_JUnitXmlFileClose(void);

#endif

