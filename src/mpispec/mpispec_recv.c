#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mpispec/mpispec_redis.h"

#define KEY_MAX_LENGTH 32

int MPI_Recv(void *buf, int count, MPI_Datatype type, int source,
             int tag, MPI_Comm comm, MPI_Status *status)
{
    int rank, cache = 0;
    char key[KEY_MAX_LENGTH];
    if (getenv("MPISPEC_CACHE") != NULL) goto leave;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    redisContext *redis = MPISpec_Redis_Connect();
    if (redis == NULL) goto leave;

    sprintf(key, "mpispec:send:%d_%d_%d", source, rank, tag);
    redisReply *res = redisCommand(redis, "EXISTS %s", key);
    if ((int)res->integer == 1) {
        cache = 1;
        res = redisCommand(redis, "GET %s", key);
        *((int *)buf) = atoi(res->str);
        status = MPI_STATUS_IGNORE;
    }
    freeReplyObject(res);

leave:
    return (cache == 1) ? MPI_SUCCESS : PMPI_Recv(buf, count, type, source, tag, comm, status);
}
