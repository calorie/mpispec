/*
 *  cspec_config.h   :  This file depends on the capabilities of your hardware/platform
 *
 * See copyright notice in cspec.h
 *
 */

#ifndef MPISPEC_CONFIG_H
#define MPISPEC_CONFIG_H

/* You can change cspec_double to an other type */
/* if your platform does not support it      */
typedef double cspec_double;
cspec_double cspec_fabs( cspec_double arg );

int cspec_strcmp ( const char * str1, const char * str2 );

typedef void ( * MPISpecFun ) ( );
typedef int mpispec_bool;
mpispec_bool mpispec_recv(MPISpecFun fun, int from, int to, int tag, double timeout );

#endif

