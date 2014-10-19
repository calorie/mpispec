#include <hiredis/hiredis.h>
#include <mpi.h>
#include "mpispec_redis.h"

redisContext *
MPISpec_Redis_Connect()
{
    static redisContext *connection = NULL;
    if (connection != NULL) return connection;

    connection = redisConnect("127.0.0.1", 6379);
    if (connection != NULL && connection->err) {
        printf("Error: %s\n", connection->errstr);
        redisFree(connection);
        connection = NULL;
    }
    return connection;
}

void
MPISpec_Redis_Disconnect()
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    redisContext *connection = MPISpec_Redis_Connect();
    if (connection != NULL) redisFree(connection);
}
