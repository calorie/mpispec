/*
 *  cspec_output_xml.h   :  Declaration of the xml output
 *
 * See copyright notice in cspec.h
 *
 */

#ifndef MPISPEC_OUTPUT_XML_H
#define MPISPEC_OUTPUT_XML_H

#include "mpispec_output.h"

CSpecOutputStruct* CSpec_NewOutputXml();

void MPISpec_XmlFileOpen(const char *filename, const char *encoding);
void MPISpec_XmlFileClose(void);

#endif
