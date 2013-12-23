
/* Time-stamp: <2007-11-14 22:31:01 shinya> */

/*
 * Original:
 *
 * Argonne National Laboratory Group
 * W. Gropp
 * E. Lusk
 * Mathematics and Computer Science Division
 * Argonne National Laboratory, Argonne IL 60439

 * Mississippi State Group
 * N. Doss
 * A. Skjellum
 * Mississippi State University, Computer Science Department &
 * NSF Engineering Research Center for Computational Field Simulation
 * P.O. Box 6176, Mississippi State MS 39762
 *
 * Reproduce:
 *
 * Shinya Abe
 * Student of Graduate School of Engineering, Iwate University
 * Email : midmind@mail.goo.ne.jp
 */

#include "mpi.h"
#include <stdio.h>
#include <math.h>

static double
f(double x)
{
  return (4.0 / (1.0 + x * x));
}

double
cpi()
{
  int done = 0, n, myid, numprocs, i;
  double mypi, pi, h, sum, x;

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  n = 0;
  while (!done) {
    if (myid == 0) {
      if (n == 0) {
        n=10;
      }
      else {
        n=0;
      }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n == 0) {
      done = 1;
    }
    else {
      h   = 1.0 / (double) n;
      sum = 0.0;
      for (i = myid + 1; i <= n; i += numprocs) {
        x = h * ((double)i - 0.5);
        sum += f(x);
      }
      mypi = h * sum;

      MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

      if (myid == 0) {
        return pi;
      }
      else {
        return 0.0;
      }
    }
  }
  return 0.0;
}
