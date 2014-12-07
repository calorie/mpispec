#include <mpi.h>
#include <stdio.h>

int async() {
    int rank, val = 0;
    MPI_Request request;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        MPI_Irecv(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    }
    if (rank == 0) {
        val = 1;
        MPI_Isend(&val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    }

    MPI_Wait(&request, &status);

    return val;
}
