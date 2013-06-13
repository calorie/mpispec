
/* Time-stamp: <2007-11-14 22:38:21 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include <mpi.h>

#define NANOSEC_MAX 300000

__attribute__((constructor))
static void
delay_setup()
{
  srandom(getpid());
}

__attribute__ ((no_instrument_function))
static void
delay();

__attribute__ ((no_instrument_function))
static void
delay()
{
  struct timespec req;

  req.tv_sec = 0;
  req.tv_nsec = random() % NANOSEC_MAX;
  nanosleep(&req, NULL);
}

__attribute__ ((no_instrument_function))
void
__cyg_profile_func_enter(void *func_addr, void *callsite)
{
  if(random() % 2) {
    delay();
  }
}

__attribute__ ((no_instrument_function))
void
__cyg_profile_func_exit(void *func_addr, void *callsite)
{
}
