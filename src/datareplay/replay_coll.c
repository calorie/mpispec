
/* Time-stamp: <2007-11-14 22:35:31 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <replay_common.h> 

int
MPI_Bcast(void* buf, int count, MPI_Datatype datatype,
          int root, MPI_Comm comm)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datareplay_replay(buf, size, count);
  
  return ret;
}

int
MPI_Scatter(void* sendbuf, int sendcnt, MPI_Datatype sendtype,
            void* recvbuf, int recvcnt, MPI_Datatype recvtype,
            int root, MPI_Comm comm)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(recvtype, &size);
  datareplay_replay(recvbuf, size, recvcnt);
  
  return ret;
}

int
MPI_Gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype,
           void* recvbuf, int recvcnt, MPI_Datatype recvtype,
           int root, MPI_Comm comm)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(recvtype, &size);
  datareplay_replay(recvbuf, size, recvcnt);

  return ret;
}

int
MPI_Reduce(void* operand, void* result, int count, MPI_Datatype datatype,
           MPI_Op operator, int root, MPI_Comm comm)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datareplay_replay(result, size, count);

  return ret;
}

int
MPI_Allreduce(void* operand, void* result, int count,
              MPI_Datatype datatype, MPI_Op operator, MPI_Comm comm)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datareplay_replay(result, size, count);

  return ret;
}

int
MPI_Barrier(MPI_Comm comm)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);

  return ret;
}
