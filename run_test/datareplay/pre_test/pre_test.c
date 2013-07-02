#include <mpi.h>

int
main(int argc, char **argv)
{
  int myrank, n_procs;
  MPI_Status st;
  MPI_Request sendreq, recvreq;
  int sdata, rdata;
  int ary_sdata[3], ary_rdata[3];

  MPI_Init(&argc, &argv);

  // pt2pt
  sdata = 1;
  rdata = 0;
  MPI_Send(&sdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Recv(&rdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);

/*   sdata = 2; */
/*   rdata = 0; */
/*   MPI_Isend(&sdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &sendreq); */
/*   MPI_Irecv(&rdata, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &recvreq); */
/*   MPI_Wait(&sendreq, &st); */
/*   MPI_Wait(&recvreq, &st); */

  // coll
  sdata = 3;
  rdata = 0;
  MPI_Bcast(&sdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

  sdata = 4;
  rdata = 0;
  MPI_Scatter(&sdata, 1, MPI_INT, &rdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

  sdata = 5;
  rdata = 0;
  MPI_Gather(&sdata, 1, MPI_INT, &rdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

  sdata = 6;
  rdata = 0;
  MPI_Reduce(&sdata, &rdata, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  sdata = 7;
  rdata = 0;
  MPI_Reduce(&sdata, &rdata, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // context
  MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  
  MPI_Finalize();
}
