/*
 *  cspec_config.c   :  This file depends on the capabilities of your hardware/platform
 *
 * In case your platform does not support double or provides another function to compute the absolute
 * value of a double, you can customize in this file.
 *
 * TODO: add a comparison function for cspec_double
 *
 * See copyright notice in cspec.h
 *
 */

#include <string.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "cspec_config.h"

#define MS_TRUE 1
#define MS_FALSE 0
#define MS_DEFAULT_TIMEOUT 2

int cspec_strcmp ( const char * str1, const char * str2 )
{
  return strcmp(str1, str2);
}

cspec_double cspec_fabs( cspec_double arg )
{
  return fabs(arg);
}

mpispec_bool mpispec_send_recv(MPISpecFun fun, int from, int to, int tag, int timeout )
{
  int i = 0;
  int test = MS_FALSE;
  int myrank;
  MPI_Status status;
  struct sigaction sa;
  struct itimerval itimer;
  int a = 1;

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  void mywait(int no) {}
  memset(&sa,0,sizeof(struct sigaction));
  sa.sa_handler = mywait;
  if(sigaction(SIGALRM,&sa,NULL) != 0)
    return test;
  itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = timeout;
  itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0;
  if(setitimer(ITIMER_REAL,&itimer,NULL) < 0)
    return test;

  if (myrank == to) {
    if (timeout < 0)
      timeout = MS_DEFAULT_TIMEOUT;

    while (i <= timeout) {
      MPI_Iprobe(from, tag, MPI_COMM_WORLD, &test, &status);

      if (test == MS_TRUE)
        break;

      sleep(1);
      i++;
    }
  }

  fun();

  if (myrank == to)
    MPI_Send(&test, 1, MPI_INT, from, 0, MPI_COMM_WORLD);
  else if(myrank == from)
    MPI_Recv(&test, 1, MPI_INT, to, 0, MPI_COMM_WORLD, &status);

  return test;
}
