#include <mpi.h>
#include <cue/cue.h>

CU_DEF(test_coll)
{
  int myrank;
  int sdata, rdata;
  int ary_sdata[3], ary_rdata[3];
  
  PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  
  sdata = 3;
  rdata = 0;
  MPI_Bcast(&sdata, 1, MPI_INT, myrank, MPI_COMM_WORLD);
  CU_ASSERT_EQUAL(sdata, 3);

  sdata = 4;
  rdata = 0;
  MPI_Scatter(&sdata, 1, MPI_INT, &rdata, 1, MPI_INT, myrank, MPI_COMM_WORLD);
  CU_ASSERT_EQUAL(rdata, 4);

  sdata = 5;
  rdata = 0;
  MPI_Gather(&sdata, 1, MPI_INT, &rdata, 1, MPI_INT, myrank, MPI_COMM_WORLD);
  CU_ASSERT_EQUAL(rdata, 5);

  sdata = 6;
  rdata = 0;
  MPI_Reduce(&sdata, &rdata, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  CU_ASSERT_EQUAL(rdata, 6);

  sdata = 7;
  rdata = 0;
  MPI_Reduce(&sdata, &rdata, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  CU_ASSERT_EQUAL(rdata, 7);
}
