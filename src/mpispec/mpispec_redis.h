#ifndef MPISPEC_REDIS_H
#define MPISPEC_REDIS_H

#include <hiredis/hiredis.h>

redisContext *MPISpec_Redis_Connect();
void MPISpec_Redis_Disconnect();

#endif
