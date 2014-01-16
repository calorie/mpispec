#include <mpi.h>
#include <cue/cue.h>

CU_DEF(test_pt2pt)
{
  int myrank;
  MPI_Status st;
  MPI_Request sendreq, recvreq;
  int sdata, rdata;

  sdata = 1;
  rdata = 0;
  MPI_Send(&sdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Recv(&rdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
  CU_ASSERT_EQUAL(rdata, 1);

/*   sdata = 2; */
/*   rdata = 0; */
/*   MPI_Isend(&sdata, 1, MPI_INT, myrank, 0, MPI_COMM_WORLD, &sendreq); */
/*   MPI_Irecv(&rdata, 1, MPI_INT, myrank, 0, MPI_COMM_WORLD, &recvreq); */
/*   MPI_Wait(&sendreq, &st); */
/*   MPI_Wait(&recvreq, &st); */
/*   CU_ASSERT_EQUAL(rdata, 0); */
}
