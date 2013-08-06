#include <mpi.h>
#include <cue/cue.h>

CU_DEF(test_context)
{
  int myrank, n_procs;
  int sdata, rdata;
  int ary_sdata[3], ary_rdata[3];
  

  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
  CU_ASSERT_EQUAL(n_procs, 1);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  CU_ASSERT_EQUAL(myrank, 0);

}
