#include <stdlib.h>
#include <mpi.h>
#include "mpispec_comm_world.h"

MPI_Comm *MPISpec_Comm_World() {
    static MPI_Comm *comm;
    if (comm != NULL) return comm;
    comm = (MPI_Comm *)malloc(sizeof(MPI_Comm));
    MPI_Comm_dup(MPI_COMM_WORLD, comm);
    return comm;
}

void MPISpec_Comm_World_Free() { MPI_Comm_free(MPISpec_Comm_World()); }
