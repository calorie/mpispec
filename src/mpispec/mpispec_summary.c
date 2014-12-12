#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpispec_output.h"
#include "mpispec_summary.h"
#include "mpispec_util.h"

#define MPISPEC_RESULT_BAR "="

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

void MPISpec_Result_File_Open() {
    char result_filename[32];
    int rank = MPISpec_Rank();

    sprintf(result_filename, "rank%d.result", rank);
    if ((MPISPEC_GLOBAL_FP = fopen(result_filename, "a")) == NULL) {
        fprintf(stderr, "Can't open result files");
        exit(-1);
    }
    fprintf(MPISPEC_GLOBAL_FP, "\nrank  %d:", rank);
}

void MPISpec_Run_Summary(void) {
    pMPISpecRunSummary summary = MPISpec_Get_Run_Summary();

    fprintf(MPISPEC_GLOBAL_FP,
            "\n--Run Summary: Type      Total  Passed  Failed"
            "\n               tests  %8u%8u%8u\n",
            summary->Total, summary->Passed, summary->Total - summary->Passed);
}

void MPISpec_Result_File_Close(void) { fclose(MPISPEC_GLOBAL_FP); }

void MPISpec_Display_Results(void) {
    unsigned int specs, successes, fails;

    get_total_results(&specs, &successes, &fails);
    if (MPISpec_Rank() != 0) return;
    display_results(MPISpec_Size(), specs, fails);
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

unsigned int mpispec_get_number_of_specs(void) {
    return MPISpec_Get_Run_Summary()->Total;
}

unsigned int mpispec_get_number_of_successes(void) {
    return MPISpec_Get_Run_Summary()->Passed;
}

unsigned int mpispec_get_number_of_failures(void) {
    pMPISpecRunSummary summary = MPISpec_Get_Run_Summary();
    return summary->Total - summary->Passed;
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
            MPISpec_Current_Time() - MPISpec_Start_Time());
}
