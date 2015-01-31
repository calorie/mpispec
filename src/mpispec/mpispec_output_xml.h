/*
 *  cspec_output_xml.h   :  Declaration of the xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#ifndef MPISPEC_OUTPUT_XML_H
#define MPISPEC_OUTPUT_XML_H

#include "mpispec_output.h"

#define MPISPEC_XML_BASE_FILENAME "output.xml"
#define MPISPEC_XML_ENCODING "utf-8"

MPISpecOutputStruct *MPISpec_NewOutputXml(void);

void MPISpec_XmlFileOpen(const char *filename, const char *encoding);
void MPISpec_XmlFileClose(void);

#endif
