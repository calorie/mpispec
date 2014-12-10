#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <ompi/request/request.h>
#include "mpispec_comm_world.h"

int MPI_Wait(MPI_Request *request, MPI_Status *status) {
    if ((*request)->req_mpi_object.comm == MPISPEC_COMM_WORLD) {
        MPI_Start(request);
    }
    return PMPI_Wait(request, status);
}
