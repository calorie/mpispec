#include <stdio.h>
#include <mpi.h>
#include "mpispec_comm_world.h"
#include "mpispec_hash.h"

int MPI_Isend(void *buf, int count, MPI_Datatype type, int dest, int tag,
              MPI_Comm comm, MPI_Request *request) {
    MPI_Request req;
    int source;
    char key[MPISPEC_HASH_KEY_MAX];
    hash_t *hash = MPISpec_Hash();
    MPI_Comm_rank(MPI_COMM_WORLD, &source);
    sprintf(key, "%d_%d_%d", source, dest, tag);
    void *stub = hash_get(hash, key);
    if (!stub) {
        return PMPI_Isend(buf, count, type, dest, tag, comm, request);
    }
    MPI_Send_init(stub, count, type, dest, tag, MPISPEC_COMM_WORLD, request);
    PMPI_Irecv(buf, count, type, dest, tag, MPISPEC_COMM_WORLD, &req);
    return MPI_SUCCESS;
}
