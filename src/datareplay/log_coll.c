
/* Time-stamp: <2007-11-14 22:35:00 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <log_common.h>
  
int
MPI_Bcast(void* buf, int count, MPI_Datatype datatype,
          int root, MPI_Comm comm)
{
  int ret, size;

  ret = PMPI_Bcast(buf, count, datatype, root, comm);
  
  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datalog_log(buf, size, count);

  return ret;
}

int
MPI_Scatter(void* sendbuf, int sendcnt, MPI_Datatype sendtype,
            void* recvbuf, int recvcnt, MPI_Datatype recvtype,
            int root, MPI_Comm comm)
{
  int ret, size;

  ret = PMPI_Scatter(sendbuf, sendcnt, sendtype,
                     recvbuf, recvcnt, recvtype,
                     root, comm);
  
  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(recvtype, &size);
  datalog_log(recvbuf, size, recvcnt);

  return ret;
}

int
MPI_Gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype,
           void* recvbuf, int recvcnt, MPI_Datatype recvtype,
           int root, MPI_Comm comm)
{
  int ret, size;

  ret = PMPI_Gather(sendbuf, sendcnt, sendtype,
                    recvbuf, recvcnt, recvtype,
                    root, comm);
  
  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(recvtype, &size);
  datalog_log(recvbuf, size, recvcnt);

  return ret;
}

int
MPI_Reduce(void* operand, void* result, int count, MPI_Datatype datatype,
           MPI_Op operator, int root, MPI_Comm comm)
{
  int ret, size;

  ret = PMPI_Reduce(operand, result, count, datatype, operator,
                    root, comm);

  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datalog_log(result, size, count);
  
  return ret;
}

int
MPI_Allreduce(void* operand, void* result, int count,
              MPI_Datatype datatype, MPI_Op operator, MPI_Comm comm)
{
  int ret, size;

  ret = PMPI_Allreduce(operand, result, count, datatype, operator, comm);

  datalog_log(&ret, sizeof(int), 1);
  MPI_Type_size(datatype, &size);
  datalog_log(result, size, count);

  return ret;
}

int
MPI_Barrier(MPI_Comm comm)
{
  int ret;

  ret = PMPI_Barrier(comm);

  datalog_log(&ret, sizeof(int), 1);

  return ret;
}
