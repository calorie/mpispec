
/* Time-stamp: <2007-11-14 22:35:42 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <replay_common.h> 

int
MPI_Comm_size(MPI_Comm comm, int* numprocs)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(numprocs, sizeof(int), 1);
  
  return ret;
}

int
MPI_Comm_rank(MPI_Comm comm, int* rank)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(rank, sizeof(int), 1);
  
  return ret;
}

int
MPI_Cart_create(MPI_Comm oldcomm, int ndims, int dimsizes[],
                int wraparound[], int reorder, MPI_Comm* cart)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(cart, sizeof(MPI_Comm), 1);

  return ret;
}

int
MPI_Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[],
             int coords[])
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(dims, sizeof(int), maxdims);
  datareplay_replay(periods, sizeof(int), maxdims);
  datareplay_replay(coords, sizeof(int), maxdims);

  return ret;
}

int
MPI_Cart_shift(MPI_Comm comm, int direction, int displacement,
               int* ranksrc, int* rankdest)
{
  int ret;

  datareplay_replay(&ret, sizeof(int), 1);
  datareplay_replay(ranksrc, sizeof(int), 1);
  datareplay_replay(rankdest, sizeof(int), 1);

  return ret;
}
