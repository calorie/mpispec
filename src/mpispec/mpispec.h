/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on
 *CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 */

/* Time-stamp: <2007-11-14 22:36:36 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

/*
 *  cspec.h   :   the main header file
 *
 * See copyright notice at the end of this file
 *
 */

#ifndef MPISPEC_H
#define MPISPEC_H

#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include "mpispec_basic.h"
#include "mpispec_config.h"
#include "mpispec_private.h"
#include "mpispec_output.h"
#include "mpispec_output_verbose.h"
#include "mpispec_output_junit_xml.h"
#include "mpispec_output_xml.h"
#include "mpispec_runner.h"
#include "mpispec_stub.h"

void MPISpec_Setup(int argc, char **argv);
void MPISpec_Dispatch(void);
int MPISpec_Rank(void);

/*               */
/* Public macros */
/*               */

/* Runner macros */

#define MPISpec_VerboseRun(test) \
    void test();                 \
    MPISpec_Run(test, MPISpec_NewOutputVerbose());

#define MPISpec_JUnitXmlRun(test)                          \
    void test();                                           \
    MPISpec_JUnitXmlFileOpen("junit_output.xml", "utf-8"); \
    MPISpec_Run(test, MPISpec_NewOutputJUnitXml());

#define MPISpec_XmlRun(test)                    \
    void test();                                \
    MPISpec_XmlFileOpen("output.xml", "utf-8"); \
    MPISpec_Run(test, MPISpec_NewOutputXml());

/* Config macros */

#define MPISPEC_INIT                  \
    int main(int argc, char **argv) { \
        MPISpec_Setup(argc, argv);

#define MPISPEC_FINALIZE \
    MPISpec_Dispatch();  \
    return EXIT_SUCCESS; \
    }

/* Structural macros */

#define RANK(rank)           \
    {                        \
        MPISpec_StartRank(); \
        if (MPISpec_Rank() == rank) {
#define END_RANK       \
    }                  \
    MPISpec_EndRank(); \
    }

#define EX_RANK(rank)        \
    {                        \
        MPISpec_StartRank(); \
        if (MPISpec_Rank() != rank) {
#define END_EX_RANK    \
    }                  \
    MPISpec_EndRank(); \
    }

#define RANKS(ranks)                                                       \
    {                                                                      \
        MPISpec_StartRanks();                                              \
        if (MPISpec_ValidateRanks(ranks, sizeof(ranks) / sizeof(ranks[0]), \
                                  MPISpec_Rank()) == 0) {
#define END_RANKS       \
    }                   \
    MPISpec_EndRanks(); \
    }

#define EX_RANKS(ranks)                                                    \
    {                                                                      \
        MPISpec_StartRanks();                                              \
        if (MPISpec_ValidateRanks(ranks, sizeof(ranks) / sizeof(ranks[0]), \
                                  MPISpec_Rank()) != 0) {
#define END_EX_RANKS    \
    }                   \
    MPISpec_EndRanks(); \
    }

#define MPISPEC_DEF(test)   \
    void test() {           \
        MPISpec_StartDef(); \
        {
#define END_DEF       \
    }                 \
    MPISpec_EndDef(); \
    }

#define DESCRIBE(caption)               \
    {                                   \
        MPISpec_StartDescribe(caption); \
        {
#define END_DESCRIBE       \
    }                      \
    MPISpec_EndDescribe(); \
    }

#define CONTEXT(caption) DESCRIBE(caption)
#define END_CONTEXT END_DESCRIBE

#define IT(caption)               \
    {                             \
        MPISpec_StartIt(caption); \
        {
#define END_IT       \
    }                \
    MPISpec_EndIt(); \
    }

/* gcc only */
#ifndef __clang__

#define BEFORE_EACH(foo)                   \
    auto void before_each_##foo(void);     \
    MPISpec_Set_Before(before_each_##foo); \
    void before_each_##foo() {             \
        MPISpec_StartBefore();             \
        {
#define END_BEFORE       \
    }                    \
    MPISpec_EndBefore(); \
    }

#define AFTER_EACH(foo)                  \
    auto void after_each_##foo(void);    \
    MPISpec_Set_After(after_each_##foo); \
    void after_each_##foo() {            \
        MPISpec_StartAfter();            \
        {
#define END_AFTER       \
    }                   \
    MPISpec_EndAfter(); \
    }

#endif

#define END        \
    }              \
    MPISpec_End(); \
    }

/* Expectation macros */

#define SHOULD_BE_TRUE(x) MPISPEC_EVAL((x))
#define SHOULD_EQUAL(x, y) MPISPEC_EVAL((x) == (y))
#define SHOULD_EQUAL_DOUBLE(x, y, delta) \
    MPISPEC_EVAL(MPISpec_Fabs((x) - (y)) <= delta)
#define SHOULD_MATCH(x, y) MPISPEC_EVAL(MPISpec_Strcmp(x, y) == 0)
#define SHOULD_BE_NULL(x) MPISPEC_EVAL((x) == 0)

#define SHOULD_BE_FALSE(x) MPISPEC_EVAL(!(x))
#define SHOULD_NOT_EQUAL(x, y) MPISPEC_EVAL((x) != (y))
#define SHOULD_NOT_EQUAL_DOUBLE(x, y, delta) \
    MPISPEC_EVAL(MPISpec_Fabs((x) - (y)) > delta)
#define SHOULD_NOT_MATCH(x, y) MPISPEC_EVAL(MPISpec_Strcmp(x, y) != 0)
#define SHOULD_NOT_BE_NULL(x) MPISPEC_EVAL((x) != 0)

#define SHOULD_PENDING(reason) MPISPEC_PENDING(reason)

#define STUB_NON_BLOCKING_P2P(data, from, to, tag) \
    MPISpec_Stub(data, from, to, tag);

#endif

/*
 * Copyright 2008 Arnaud Brejeon.
 *
 * Cspec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * CSpec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */
