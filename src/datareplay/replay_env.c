
/* Time-stamp: <2007-11-14 22:35:47 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <replay_common.h>

int
MPI_Init(int* argc, char*** argv)
{
  int ret;
  char filename[32];

  strncpy(filename, (*argv)[*argc-1], 32);
  datareplay_open(filename);
  initialize_req_table();

  PMPI_Init(argc, argv);

  datareplay_replay(&ret, sizeof(int), 1);

  return ret;
}

int
MPI_Finalize()
{
  datareplay_close();

  return PMPI_Finalize();
}
