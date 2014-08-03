/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:04:05
 */

/*
 *  cspec_output_verbose.h   :  Declaration of the verbose output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef MPISPEC_OUTPUT_VERBOSE_H
#define MPISPEC_OUTPUT_VERBOSE_H

#include "mpispec_output.h"

MPISpecOutputStruct* MPISpec_NewOutputVerbose(void);

#endif
