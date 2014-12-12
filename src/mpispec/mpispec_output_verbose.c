/*
 *  cspec_output_verbose.c   :  Verbose output with color (green for OK, red for
 *fail and yellow for pending)
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#include <stdarg.h>
#include <stdio.h>
#include "mpispec_output_verbose.h"

typedef enum {
    MPISPEC_COLOR_RED = 1,
    MPISPEC_COLOR_GREEN = 2,
    MPISPEC_COLOR_YELLOW = 3
} MPISpec_Color;

static MPISpecOutputStruct verbose;
static int mpispec_tab_num = 0;
pMPISpecRunSummary verbose_summary;

static void fprint_tab(int n);
static void colored_fprintf(MPISpec_Color color, const char *format, ...);
static int get_ansi_color_code(MPISpec_Color color);

void start_def_fun_verbose(void) {
    verbose_summary = MPISpec_Get_Run_Summary();
}

void end_def_fun_verbose(void) {}

void start_describe_fun_verbose(const char *descr) {
    fprintf(MPISPEC_GLOBAL_FP, "\n");
    fprint_tab(++mpispec_tab_num);
    fprintf(MPISPEC_GLOBAL_FP, "%s\n", descr);
}

void end_describe_fun_verbose(void) { mpispec_tab_num--; }

void start_it_fun_verbose(const char *descr) {
    fprint_tab(++mpispec_tab_num);
    fprintf(MPISPEC_GLOBAL_FP, "- %s\n", descr);
}

void end_it_fun_verbose(void) { mpispec_tab_num--; }

void end_fun_verbose(void) { mpispec_tab_num--; }

void eval_fun_verbose(const char *filename, int line_number,
                      const char *assertion, int assertion_result) {
    fprint_tab(mpispec_tab_num + 1);
    verbose_summary->Total++;
    if (assertion_result) {
        verbose_summary->Passed++;
        colored_fprintf(MPISPEC_COLOR_GREEN, "OK: %s\n", assertion, filename,
                        line_number);
    } else {
        colored_fprintf(MPISPEC_COLOR_RED, "Failed: %s in file %s at line %d\n",
                        assertion, filename, line_number);
    }
}

void pending_fun_verbose(const char *reason) {
    colored_fprintf(MPISPEC_COLOR_YELLOW, "       Pending: %s\n", reason);
}

MPISpecOutputStruct *MPISpec_NewOutputVerbose(void) {
    MPISpec_InitOutput(&verbose);

    verbose.start_def_fun = start_def_fun_verbose;
    verbose.end_def_fun = end_def_fun_verbose;
    verbose.start_describe_fun = start_describe_fun_verbose;
    verbose.end_describe_fun = end_describe_fun_verbose;
    verbose.start_it_fun = start_it_fun_verbose;
    verbose.end_it_fun = end_it_fun_verbose;
    verbose.end_fun = end_fun_verbose;
    verbose.eval_fun = eval_fun_verbose;
    verbose.pending_fun = pending_fun_verbose;

    return &verbose;
}

static int get_ansi_color_code(MPISpec_Color color) {
    int color_code;

    switch (color) {
        case MPISPEC_COLOR_RED:
            color_code = 31;
            break;
        case MPISPEC_COLOR_GREEN:
            color_code = 32;
            break;
        case MPISPEC_COLOR_YELLOW:
            color_code = 33;
            break;
        default:
            color_code = 30;
            break;
    }
    return color_code;
}

static void fprint_tab(int n) {
    int i;
    for (i = 0; i < n; i++) fprintf(MPISPEC_GLOBAL_FP, MPISPEC_TAB);
}

static void colored_fprintf(MPISpec_Color color, const char *format, ...) {
    va_list args;

    va_start(args, format);

    /* Set color */
    fprintf(MPISPEC_GLOBAL_FP, "\033[0;%dm", get_ansi_color_code(color));

    /* Print Text */
    vfprintf(MPISPEC_GLOBAL_FP, format, args);

    /* Reset color */
    fprintf(MPISPEC_GLOBAL_FP, "\033[m");

    va_end(args);
    return;
}
