
/* Time-stamp: <2007-11-14 22:31:12 shinya> */

#include <mpi.h>
#include "integral.h"

static double
local_integral(double min, double max, int n, double f(double))
{
  int i;
  double x, dx, sum;

  x = min;
  dx = (max - min) / n;
  sum = (f(min) + f(max)) / 2.0;

  for(i = 1; i < n; i++) {
    x += dx;
    sum += f(x);
  }

  return sum * dx;
}

double
integral(double min, double max, int n, double f(double))
{
  int myrank, n_procs;
  MPI_Status st;
  double local_min, local_max, local_sum, sum;
  int i;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

  sum = local_sum = 0.0;

  local_min = min + ((max - min) / n_procs * myrank);
  local_max = min + ((max - min) / n_procs * (myrank + 1));
  local_sum = local_integral(local_min, local_max, n, f);

  MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return sum;
}
