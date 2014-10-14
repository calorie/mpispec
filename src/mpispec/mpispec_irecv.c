#include <stdio.h>
#include <mpi.h>

int MPI_Irecv(void *buf, int count, MPI_Datatype type, int source,
              int tag, MPI_Comm comm, MPI_Request *request)
{
    return PMPI_Irecv(buf, count, type, source, tag, comm, request);
}
