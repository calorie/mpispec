/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 05:58:02
 */

/*
 *  cspec_config.h   :  This file depends on the capabilities of your hardware/platform
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef MPISPEC_CONFIG_H
#define MPISPEC_CONFIG_H

/* You can change cspec_double to an other type */
/* if your platform does not support it      */
typedef double cspec_double;
typedef void (*MPISpecFun)();
typedef int mpispec_bool;

cspec_double cspec_fabs(cspec_double arg);
int cspec_strcmp(const char *str1, const char *str2);

#endif

