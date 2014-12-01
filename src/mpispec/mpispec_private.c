/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on
 *CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:05:26
 */

/*
 *  cspec_private.c  :   Private functions
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include <stdio.h>
#include "mpispec_private.h"
#include "mpispec_output.h"
#include "mpispec_consts.h"

#define MPISPEC_MAX_ACTION_ARRAY_SIZE 64

typedef void (*MPISpecActionArray[MPISPEC_MAX_NEST_NUM]
                                 [MPISPEC_MAX_ACTION_ARRAY_SIZE])(void);

static MPISpecOutputStruct *MPISpec_output = 0;
static MPISpecActionArray before_array, after_array;
static void (*end_fun_stack[MPISPEC_MAX_NEST_NUM + 1])(void);
static unsigned int nest_num = 0;

static void mpispec_set_action(MPISpecActionArray aarray,
                               MPISpecTmpFunction fun);
static void mpispec_remove_action(MPISpecActionArray aarray);
static void mpispec_run_action(MPISpecActionArray aarray);

static void mpispec_remove_before(void);
static void mpispec_run_before(void);

static void mpispec_remove_after(void);
static void mpispec_run_after(void);

static void mpispec_push_end_fun(MPISpecTmpFunction end_fun);
static void mpispec_pop_end_fun(void);

int MPISpec_StartRank(void) {
    mpispec_push_end_fun(MPISpec_EndRank);
    return 0;
}

void MPISpec_EndRank(void) {}

int MPISpec_StartRanks(void) {
    mpispec_push_end_fun(MPISpec_EndRanks);
    return 0;
}

void MPISpec_EndRanks(void) {}

int MPISpec_ValidateRanks(int ranks[], const int size, int rank) {
    int i, result = 1;

    for (i = 0; i < size; i++) {
        if (i >= MPISPEC_MAX_RANKS_NUM) {
            printf("\033[1;33mWARN : ranks size over max size.\033[0m\n");
            break;
        }
        if (ranks[i] == rank) {
            result = 0;
            break;
        }
    }
    return result;
}

int MPISpec_StartDef(void) {
    if (MPISpec_output->start_def_fun != NULL) MPISpec_output->start_def_fun();
    mpispec_push_end_fun(MPISpec_EndDef);
    return 0;
}

void MPISpec_EndDef(void) {
    if (MPISpec_output->end_def_fun != NULL) MPISpec_output->end_def_fun();
}

int MPISpec_StartDescribe(const char *descr) {
    nest_num++;
    if (MPISpec_output->start_describe_fun != NULL)
        MPISpec_output->start_describe_fun(descr);
    mpispec_push_end_fun(MPISpec_EndDescribe);
    return 0;
}

void MPISpec_EndDescribe(void) {
    mpispec_remove_before();
    mpispec_remove_after();
    nest_num--;
    if (MPISpec_output->end_describe_fun != NULL)
        MPISpec_output->end_describe_fun();
}

int MPISpec_StartIt(const char *descr) {
    mpispec_run_before();
    if (MPISpec_output->start_it_fun != NULL)
        MPISpec_output->start_it_fun(descr);
    mpispec_push_end_fun(MPISpec_EndIt);
    return 0;
}

void MPISpec_EndIt(void) {
    mpispec_run_after();
    if (MPISpec_output->end_it_fun != NULL) MPISpec_output->end_it_fun();
}

int MPISpec_StartBefore(void) {
    mpispec_push_end_fun(MPISpec_EndBefore);
    return 0;
}

void MPISpec_EndBefore(void) {}

int MPISpec_StartAfter(void) {
    mpispec_push_end_fun(MPISpec_EndAfter);
    return 0;
}

void MPISpec_EndAfter(void) {}

void MPISpec_End(void) { mpispec_pop_end_fun(); }

void MPISpec_Eval(const char *filename, int line_number, const char *assertion,
                  int assertion_result) {
    if (MPISpec_output->eval_fun != NULL)
        MPISpec_output->eval_fun(filename, line_number, assertion,
                                 assertion_result);
    if (!assertion_result) MPISpec_output->failed++;
}

void MPISpec_Pending(const char *reason) {
    if (MPISpec_output->pending_fun != NULL)
        MPISpec_output->pending_fun(reason);
}

void MPISpec_SetOutput(MPISpecOutputStruct *output) { MPISpec_output = output; }

static void mpispec_set_action(MPISpecActionArray aarray,
                               MPISpecTmpFunction fun) {
    int i;
    for (i = 0; i < MPISPEC_MAX_ACTION_ARRAY_SIZE; i++) {
        if (aarray[nest_num][i] == fun) break;
        if (aarray[nest_num][i] == NULL) {
            aarray[nest_num][i] = fun;
            break;
        }
    }
}

static void mpispec_remove_action(MPISpecActionArray aarray) {
    int i;
    for (i = 0; i < MPISPEC_MAX_ACTION_ARRAY_SIZE; i++)
        aarray[nest_num][i] = NULL;
}

static void mpispec_run_action(MPISpecActionArray aarray) {
    int i, j;
    for (i = 1; i <= nest_num; i++) {
        for (j = 0; j < MPISPEC_MAX_ACTION_ARRAY_SIZE; j++) {
            if (aarray[i][j] != NULL)
                aarray[i][j]();
            else
                break;
        }
    }
}

void MPISpec_Set_Before(MPISpecTmpFunction fun) {
    mpispec_set_action(before_array, fun);
}

void MPISpec_Set_After(MPISpecTmpFunction fun) {
    mpispec_set_action(after_array, fun);
}

static void mpispec_remove_before(void) { mpispec_remove_action(before_array); }

static void mpispec_run_before(void) { mpispec_run_action(before_array); }

static void mpispec_remove_after(void) { mpispec_remove_action(after_array); }

static void mpispec_run_after(void) { mpispec_run_action(after_array); }

static void mpispec_push_end_fun(MPISpecTmpFunction end_fun) {
    int i;
    for (i = 0; i < MPISPEC_MAX_NEST_NUM; i++) {
        if (end_fun_stack[i] == NULL) {
            end_fun_stack[i] = end_fun;
            break;
        }
    }
}

static void mpispec_pop_end_fun(void) {
    int i;
    for (i = 1; i <= MPISPEC_MAX_NEST_NUM; i++)
        if (end_fun_stack[i] == NULL) break;
    if (end_fun_stack[i - 1] != NULL) {
        end_fun_stack[i - 1]();
        end_fun_stack[i - 1] = NULL;
    }
}
