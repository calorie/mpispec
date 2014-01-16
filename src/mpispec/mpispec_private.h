/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:05:59
 */

/*
 *  cspec_private.h
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#ifndef MPISPEC_PRIVATE_H
#define MPISPEC_PRIVATE_H

int
MPISpec_StartRank();
void
MPISpec_EndRank();

int
MPISpec_StartRanks();
void
MPISpec_EndRanks();

int
MPISpec_ValidateRanks( int ranks[], const int size, int myrank );

int
MPISpec_StartDef();
void
MPISpec_EndDef();

int
CSpec_StartDescribe( const char *descr );
void
CSpec_EndDescribe();

int
CSpec_StartIt( const char *descr );
void
CSpec_EndIt();

int
MPISpec_StartBefore();
void
MPISpec_EndBefore();

int
MPISpec_StartAfter();
void
MPISpec_EndAfter();

void
CSpec_End();

void
CSpec_Eval( const char*filename, int line_number,
            const char*assertion, int assertionResult );
void
CSpec_Pending( const char* reason );

#define CSPEC_EVAL(x)         { CSpec_Eval(__FILE__, __LINE__, #x, (x)); }
#define CSPEC_PENDING(reason) { CSpec_Pending(reason); }

typedef void ( *MPISpecTmpFunction )();

void
MPISpec_set_before( MPISpecTmpFunction fun );

void
MPISpec_set_after( MPISpecTmpFunction fun );

#endif
