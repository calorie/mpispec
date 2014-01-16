/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:02:51
 */

/*
 *  cspec_output_juni_xml.h   :  Declaration of the junit xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef MPISPEC_OUTPUT_JUNIT_XML_H
#define MPISPEC_OUTPUT_JUNIT_XML_H

#include "mpispec_output.h"

CSpecOutputStruct* CSpec_NewOutputJUnitXml();

void MPISpec_JUnitXmlFileOpen(const char *filename, const char *encoding);
void MPISpec_JUnitXmlFileClose(void);

#endif

