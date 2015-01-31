/*
 *  cspec_output_juni_xml.h   :  Declaration of the junit xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#ifndef MPISPEC_OUTPUT_JUNIT_XML_H
#define MPISPEC_OUTPUT_JUNIT_XML_H

#include "mpispec_output.h"

#define MPISPEC_JUNIT_XML_BASE_FILENAME "junit_output.xml"
#define MPISPEC_JUNIT_XML_ENCODING "utf-8"

MPISpecOutputStruct *MPISpec_NewOutputJUnitXml(void);

void MPISpec_JUnitXmlFileOpen(const char *filename, const char *encoding);
void MPISpec_JUnitXmlFileClose(void);

#endif
