#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mpispec/mpispec_redis.h"

#define KEY_MAX_LENGTH 32

int MPI_Send(void *buf, int count, MPI_Datatype type, int dest,
             int tag, MPI_Comm comm)
{
    int rank, cache = 0;
    char key[KEY_MAX_LENGTH];
    if (getenv("MPISPEC_CACHE") != NULL) goto leave;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    redisContext *redis = MPISpec_Redis_Connect();
    if (redis == NULL) goto leave;

    sprintf(key, "mpispec:send:%d_%d_%d", rank, dest, tag);
    redisReply *res = redisCommand(redis, "SET %s %d", key, *((int *)buf));
    freeReplyObject(res);

leave:
    return (cache == 1) ? MPI_SUCCESS : PMPI_Send(buf, count, type, dest, tag, comm);
}
