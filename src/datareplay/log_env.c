
/* Time-stamp: <2007-11-14 22:35:24 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <log_common.h>
 
int
MPI_Init(int* argc, char*** argv)
{
  int ret, rank;
  char filename[32];

  ret = PMPI_Init(argc, argv);

  PMPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sprintf(filename, "rank%d.log", rank);
  datalog_open(filename);
  initialize_req_table();

  datalog_log(&ret, sizeof(int), 1);

  return ret;
}

int
MPI_Finalize()
{
  datalog_close();

  return PMPI_Finalize();
}
