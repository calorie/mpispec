#include <stdio.h>
#include <mpi.h>

int MPI_Isend(void *buf, int count, MPI_Datatype type, int dest,
              int tag, MPI_Comm comm, MPI_Request *request)
{
    return PMPI_Isend(buf, count, type, dest, tag, comm, request);
}
