/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 05:57:45
 */

/*
 *  cspec_config.c   :  This file depends on the capabilities of your hardware/platform
 *
 * In case your platform does not support double or provides another function to compute the absolute
 * value of a double, you can customize in this file.
 *
 * TODO: add a comparison function for cspec_double
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include <string.h>
#include <math.h>

#include "mpispec_config.h"

int
MPISpec_Strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

double
MPISpec_Fabs(double arg)
{
    return fabs(arg);
}
