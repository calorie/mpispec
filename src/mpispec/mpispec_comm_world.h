#ifndef MPISPEC_COMM_WORLD_H
#define MPISPEC_COMM_WORLD_H

#include <mpi.h>

MPI_Comm *MPISpec_Comm_World(void);
void MPISpec_Comm_World_Free();

#define MPISPEC_COMM_WORLD *MPISpec_Comm_World()

#endif
