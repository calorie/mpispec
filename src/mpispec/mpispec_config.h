/*
 *  cspec_config.h   :  This file depends on the capabilities of your
 *hardware/platform
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef MPISPEC_CONFIG_H
#define MPISPEC_CONFIG_H

double MPISpec_Fabs(double arg);
int MPISpec_Strcmp(const char *str1, const char *str2);

#endif
