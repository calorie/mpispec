#ifndef MPISPEC_SUMMARY_H
#define MPISPEC_SUMMARY_H

#include <stdio.h>

typedef struct {
    unsigned int Passed;
    unsigned int Total;
} MPISpecRunSummary;

MPISpecRunSummary *MPISpec_Get_Summary(void);
void MPISpec_Free_Summary(void);
void MPISpec_Run_Summary(void);
FILE *MPISpec_Result_File(void);
void MPISpec_Result_File_Close(void);
void MPISpec_Display_Results(void);

#endif
