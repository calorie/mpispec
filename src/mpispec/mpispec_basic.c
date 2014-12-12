/* Time-stamp: <2007-11-24 23:04:56 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

/*
 *  Original:
 *
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2004, 2005  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Implementation for basic test runner interface.
 *
 *  Created By  : Jerry St.Clair  (11-Aug-2004)
 *  Comment     : Initial implementation of basic test runner interface
 *  EMail       : jds2@users.sourceforge.net
 *
 *  Modified    : 8-Jan-2005 (JDS)
 *  Comment     : Fixed reporting bug (bug report cunit-Bugs-1093861).
 *  Email       : jds2@users.sourceforge.net
 *
 *  Modified    : 30-Apr-2005 (JDS)
 *  Comment     : Added notification of suite cleanup failure.
 *  Email       : jds2@users.sourceforge.net
 */

/** @file
 * Basic interface with output to stdout.
 */
/** @addtogroup Basic
 * @{
 */

#include <mpi.h>
#include "mpispec_basic.h"
#include "mpispec_comm_world.h"
#include "mpispec_hash.h"
#include "mpispec_output_junit_xml.h"
#include "mpispec_output_xml.h"
#include "mpispec_summary.h"
#include "mpispec_util.h"

void MPISpec_Init(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPISpec_Comm_World();
    MPISpec_Start_Time();
}

void MPISpec_Finalize(void) {
    MPISpec_JUnitXmlFileClose();
    MPISpec_XmlFileClose();
    MPISpec_Run_Summary();
    MPISpec_Result_File_Close();
    MPISpec_Hash_Del();
    MPI_Barrier(MPI_COMM_WORLD);
    MPISpec_Display_Results();
    MPISpec_Free_Summary();
    MPISpec_Comm_World_Free();
    MPI_Finalize();
}
