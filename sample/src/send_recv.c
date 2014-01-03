#include <stdio.h>
#include <math.h>

#include "mpi.h"
/* #include "mpispec/mpi_stubs_c.h" */

void send_recv()
{
  int rank;
  int a, b;
  int tag = 0;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  a = 0;
  b = 0;
  if (rank == 0) {
    a = 1;
    MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (rank == 1) {
    MPI_Recv(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
}
