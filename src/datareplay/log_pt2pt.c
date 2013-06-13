
/* Time-stamp: <2007-11-14 22:35:27 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <log_common.h>

int
MPI_Send(void* buf, int count, MPI_Datatype datatype,
         int dest, int tag, MPI_Comm comm)
{
  int ret;

  ret = PMPI_Send(buf, count, datatype, dest, tag, comm);
  
  datalog_log(&ret, sizeof(int), 1);
  
  return ret;
}

int
MPI_Recv(void* buf, int count, MPI_Datatype datatype,
         int src, int tag, MPI_Comm comm, MPI_Status *status)
{
  int ret, size;
  
  ret = PMPI_Recv(buf, count, datatype, src, tag, comm, status);
  
  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datalog_log(buf, size, count);
  datalog_log(status, sizeof(MPI_Status), 1);

  return ret;
}

int
MPI_Isend(void* buf, int count, MPI_Datatype datatype, int dest,
          int tag, MPI_Comm comm, MPI_Request* req)
{
  int ret;

  ret = PMPI_Isend(buf, count, datatype, dest, tag, comm, req);

  datalog_log(&ret, sizeof(int), 1);

  return ret;
}

int
MPI_Irecv(void* buf, int count, MPI_Datatype datatype, int src,
          int tag, MPI_Comm comm, MPI_Request* req)
{
  int ret, size;

  ret = PMPI_Irecv(buf, count, datatype, src, tag, comm, req);
  datalog_log(&ret, sizeof(int), 1);

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

  ret = PMPI_Wait(req, st);

  delete_req_table(req, &buf, &count, &size);
  datalog_log(buf, size, count);

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(st, sizeof(MPI_Status), 1);

  return ret;
}

int
MPI_Waitall(int arysize, MPI_Request req[], MPI_Status st[])
{
  int ret;
  void *buf;
  int count, size;
  int i;

  ret = PMPI_Waitall(arysize, req, st);

  for(i = 0; i< arysize; i++) {
    delete_req_table(&req[i], &buf, &count, &size);
    datalog_log(buf, size, count);
  }

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(st, sizeof(MPI_Status), arysize);

  return ret;
}
