#ifndef MPISPEC_ERROR_H
#define MPISPEC_ERROR_H

typedef int MPISpecError;
typedef MPISpecError (*MPISpecErrorFun)(void);

#define MPISPEC_ERROR (MPISpecError)1

MPISpecError MPISpec_Error(void);
void MPISpec_Init_Errors(void);
MPISpecError MPISpec_Set_Error(void);
void MPISpec_Set_Error_Fun(MPISpecErrorFun fun);
void MPISpec_Run_Errors(void);
void MPISpec_Error_Message(char *msg);

#endif
