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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include "mpispec_basic.h"
#include "mpispec_output.h"

#define MPISPEC_RESULT_BAR "="

static double gettimeofday_sec(void);
static void mpispec_make_result_file(int *myrank);
static void get_total_results(unsigned int *total_specs,
                              unsigned int *total_successes,
                              unsigned int *total_fails);
static void get_local_results(unsigned int *local_specs,
                              unsigned int *local_successes,
                              unsigned int *local_fails);
static unsigned int mpispec_get_number_of_specs(void);
static unsigned int mpispec_get_number_of_successes(void);
static unsigned int mpispec_get_number_of_failures(void);
static void display_results(int procs, unsigned int specs, unsigned int fails);
static void display_test_results(int procs);
static void display_successes_rate(int procs, unsigned int specs,
                                   unsigned int fails);
static void display_run_time(void);

FILE *MPISPEC_GLOBAL_FP;
static double test_start_time;
static MPISPEC_MODE run_mode = MPISPEC_NORMAL;

void MPISpec_Basic_Set_Mode(MPISPEC_MODE mode) { run_mode = mode; }

MPISPEC_MODE MPISpec_Basic_Get_Mode(void) { return run_mode; }

void MPISpec_Basic_Setup(void) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) test_start_time = gettimeofday_sec();

    MPISpec_Basic_Set_Mode(MPISPEC_VERBOSE);

    mpispec_make_result_file(&rank);
}

void MPISpec_Run_Summary(void) {
    pMPISpecRunSummary summary = get_mpi_run_summary();

    fprintf(MPISPEC_GLOBAL_FP,
            "\n--Run Summary: Type      Total  Passed  Failed"
            "\n               tests  %8u%8u%8u\n",
            summary->Total, summary->Passed, summary->Total - summary->Passed);
}

void MPISpec_Result_File_Close(void) { fclose(MPISPEC_GLOBAL_FP); }

void MPISpec_Display_Results(void) {
    int rank, procs;
    unsigned int specs, successes, fails;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    get_total_results(&specs, &successes, &fails);

    if (rank != 0) return;

    display_results(procs, specs, fails);
}

void mpispec_make_result_file(int *myrank) {
    char result_filename[32];

    MPI_Comm_rank(MPI_COMM_WORLD, myrank);
    sprintf(result_filename, "rank%d.result", *myrank);
    if ((MPISPEC_GLOBAL_FP = fopen(result_filename, "a")) == NULL) {
        fprintf(stderr, "Can't open result files");
        exit(-1);
    }
    fprintf(MPISPEC_GLOBAL_FP, "\nrank  %d:", *myrank);
}

unsigned int mpispec_get_number_of_specs(void) {
    return get_mpi_run_summary()->Total;
}

unsigned int mpispec_get_number_of_successes(void) {
    return get_mpi_run_summary()->Passed;
}

unsigned int mpispec_get_number_of_failures(void) {
    pMPISpecRunSummary summary = get_mpi_run_summary();
    return summary->Total - summary->Passed;
}

double gettimeofday_sec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

void get_total_results(unsigned int *total_specs, unsigned int *total_successes,
                       unsigned int *total_fails) {
    unsigned int local_specs;
    unsigned int local_successes;
    unsigned int local_fails;

    get_local_results(&local_specs, &local_successes, &local_fails);

    PMPI_Reduce(&local_specs, total_specs, 1, MPI_UNSIGNED, MPI_SUM, 0,
                MPI_COMM_WORLD);
    PMPI_Reduce(&local_successes, total_successes, 1, MPI_UNSIGNED, MPI_SUM, 0,
                MPI_COMM_WORLD);
    PMPI_Reduce(&local_fails, total_fails, 1, MPI_UNSIGNED, MPI_SUM, 0,
                MPI_COMM_WORLD);
}

void get_local_results(unsigned int *local_specs, unsigned int *local_successes,
                       unsigned int *local_fails) {
    *local_specs = mpispec_get_number_of_specs();
    *local_successes = mpispec_get_number_of_successes();
    *local_fails = mpispec_get_number_of_failures();
}

void display_results(int procs, unsigned int specs, unsigned int fails) {
    display_test_results(procs);
    display_successes_rate(procs, specs, fails);
    display_run_time();
}

void display_test_results(int procs) {
    int i, ch;
    char result_filename[32];
    FILE *fp;

    for (i = 0; i < procs; i++) {
        sprintf(result_filename, "rank%d.result", i);
        if (NULL == (fp = fopen(result_filename, "r"))) {
            fprintf(stderr, "Can't open result files");
            exit(-1);
        }
        while ((ch = fgetc(fp)) != EOF) {
            fputc(ch, stdout);
        }
        fclose(fp);
        remove(result_filename);
    }
}

void display_successes_rate(int procs, unsigned int specs, unsigned int fails) {
    int i;
    double rate;

    rate = (specs == 0) ? 100 : (double)(specs - fails) / (double)(specs) * 100;
    fprintf(stdout, "\n[%d Process Results]\n", procs);
    for (i = 1; i <= 50; i++) {
        if (i <= rate / 2)
            fprintf(stdout, "\033[1;32m%s\033[0m", MPISPEC_RESULT_BAR);
        else
            fprintf(stdout, "\033[1;31m%s\033[0m", MPISPEC_RESULT_BAR);
    }
    fprintf(stdout, "[%3.0lf%%]\n", rate);
}

void display_run_time(void) {
    fprintf(stdout, "\nRun Time: %f sec\n",
            gettimeofday_sec() - test_start_time);
}
