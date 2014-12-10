#include <mpi.h>
#include <stdio.h>
#include "mpispec_comm_world.h"
#include "mpispec_hash.h"

int MPI_Irecv(void *buf, int count, MPI_Datatype type, int source, int tag,
              MPI_Comm comm, MPI_Request *request) {
    MPI_Request req;
    int dest;
    char key[MPISPEC_HASH_KEY_MAX];
    hash_t *hash = MPISpec_Hash();
    MPI_Comm_rank(MPI_COMM_WORLD, &dest);
    sprintf(key, "%d_%d_%d", source, dest, tag);
    void *stub = hash_get(hash, key);
    if (!stub) {
        return PMPI_Irecv(buf, count, type, source, tag, comm, request);
    }
    MPI_Recv_init(buf, count, type, dest, tag, MPISPEC_COMM_WORLD, request);
    PMPI_Isend(stub, count, type, dest, tag, MPISPEC_COMM_WORLD, &req);
    return MPI_SUCCESS;
}
