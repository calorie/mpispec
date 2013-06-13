
/* Time-stamp: <2007-11-14 22:35:50 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <replay_common.h>

int
MPI_Send(void *buf, int count, MPI_Datatype datatype,
         int dest, int tag, MPI_Comm comm)
{
  int ret;
  
  datareplay_replay(&ret, sizeof(int), 1);
  
  return ret;
}

int
MPI_Recv(void *buf, int count, MPI_Datatype datatype,
         int src, int tag, MPI_Comm comm, MPI_Status *status)
{
  int ret, size;
  int *d;
  
  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datareplay_replay(buf, size, count);
  datareplay_replay(status, sizeof(MPI_Status), 1);

  return ret;
}

int
MPI_Isend(void* buf, int count, MPI_Datatype datatype, int dest,
          int tag, MPI_Comm comm, MPI_Request* req)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);

  return ret;
}

int
MPI_Irecv(void* buf, int count, MPI_Datatype datatype, int src,
          int tag, MPI_Comm comm, MPI_Request* req)
{
  int ret, size;

  datareplay_replay(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);

  register_req_table(req, buf, count, size);

  return ret;
}

int
MPI_Wait(MPI_Request* req, MPI_Status* st)
{
  int ret;
  void *buf;
  int count, size;

  delete_req_table(req, &buf, &count, &size);
  datareplay_replay(buf, size, count);

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(st, sizeof(MPI_Status), 1);

  return ret;
}

int
MPI_Waitall(int arysize, MPI_Request req[], MPI_Status st[])
{
  int ret;
  void *buf;
  int count, size;
  int i;

  for(i = 0; i< arysize; i++) {
    delete_req_table(&req[i], &buf, &count, &size);
    datareplay_replay(buf, size, count);
  }

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(st, sizeof(MPI_Status), arysize);

  return ret;
}
