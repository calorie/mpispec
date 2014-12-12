#include <mpi.h>
#include <stdio.h>
#include "mpispec_comm_world.h"
#include "mpispec_hash.h"
#include "mpispec_util.h"

int MPI_Isend(const void *buf, int count, MPI_Datatype type, int dest, int tag,
              MPI_Comm comm, MPI_Request *request) {
    MPI_Request req;
    char key[MPISPEC_HASH_KEY_MAX];
    hash_t *hash = MPISpec_Hash();
    sprintf(key, "%d_%d_%d", MPISpec_Rank(), dest, tag);
    void *stub = hash_get(hash, key);
    if (!stub) {
        return PMPI_Isend(buf, count, type, dest, tag, comm, request);
    }
    MPI_Send_init(stub, count, type, dest, tag, MPISPEC_COMM_WORLD, request);
    PMPI_Irecv(buf, count, type, dest, tag, MPISPEC_COMM_WORLD, &req);
    return MPI_SUCCESS;
}
