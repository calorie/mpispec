#include <mpi.h>
#include <stdlib.h>
#include "mpispec_basic.h"
#include "mpispec_error.h"
#include "mpispec_error_function.h"
#include "mpispec_comm_world.h"

MPI_Comm *MPISpec_Comm_World() {
    static MPI_Comm *comm;
    if (comm != NULL) return comm;
    if (MPISpec_Error()) return NULL;
    if ((comm = (MPI_Comm *)malloc(sizeof(MPI_Comm))) == NULL) {
        MPISpec_Set_Error_Fun(MPISpec_Alloc_Error);
        MPISpec_Finalize();
        exit(1);
    }
    MPI_Comm_dup(MPI_COMM_WORLD, comm);
    return comm;
}

void MPISpec_Comm_World_Free() {
    MPI_Comm *comm = MPISpec_Comm_World();
    if (comm) MPI_Comm_free(comm);
}
