
/* Time-stamp: <2007-11-14 22:35:20 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <log_common.h>

int
MPI_Comm_size(MPI_Comm comm, int* numprocs)
{
  int ret;

  ret = PMPI_Comm_size(comm, numprocs);
  
  datalog_log(&ret, sizeof(int), 1);
  datalog_log(numprocs, sizeof(int), 1);

  return ret;
}

int
MPI_Comm_rank(MPI_Comm comm, int* rank)
{

  int ret;

  ret = PMPI_Comm_rank(comm, rank);

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(rank, sizeof(int), 1);
  
  return ret;
}

int
MPI_Cart_create(MPI_Comm oldcomm, int ndims, int dimsizes[],
                int wraparound[], int reorder, MPI_Comm* cart)
{
  int ret;

  ret = PMPI_Cart_create(oldcomm, ndims, dimsizes, wraparound,
                         reorder, cart);

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(cart, sizeof(MPI_Comm), 1);

  return ret;
}

int
MPI_Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[],
             int coords[])
{
  int ret;

  ret = PMPI_Cart_get(comm, maxdims, dims, periods, coords);

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(dims, sizeof(int), maxdims);
  datalog_log(periods, sizeof(int), maxdims);
  datalog_log(coords, sizeof(int), maxdims);

  return ret;
}

int
MPI_Cart_shift(MPI_Comm comm, int direction, int displacement,
               int* ranksrc, int* rankdest)
{
  int ret;

  ret = PMPI_Cart_shift(comm, direction, displacement, ranksrc, rankdest);

  datalog_log(&ret, sizeof(int), 1);
  datalog_log(ranksrc, sizeof(int), 1);
  datalog_log(rankdest, sizeof(int), 1);

  return ret;
}
