#ifndef MPISTUB_H
#define MPISTUB_H

#include "mpi.h"

# define MPI_FAILURE 1
# define MPI_SUCCESS 0
# define MPI_PRODUCT 4

int stub_MPI_Allgather ( void *sendbuf, int sendcount, MPI_Datatype sendtype,
  void *recvbuf, int recvcount, MPI_Datatype recvtype,
  MPI_Comm comm );
int stub_MPI_Allgatherv ( void *sendbuf, int sendcount, MPI_Datatype sendtype,
  void *recvbuf, int *recvcounts, int *displs,
  MPI_Datatype recvtype, MPI_Comm comm );
int stub_MPI_Allreduce ( void *sendbuf, void *recvbuf, int count,
  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm );
int stub_MPI_Barrier ( MPI_Comm comm );
int stub_MPI_Bcast ( void *data, int n, MPI_Datatype datatype, int node,
  MPI_Comm comm );
int mpi_copy_byte ( char *data1, char *data2, int n );
int mpi_copy_double ( double *data1, double *data2, int n );
int mpi_copy_float ( float *data1, float *data2, int n );
int mpi_copy_int ( int *data1, int *data2, int n );
int stub_MPI_Irecv ( void *buf, int count, MPI_Datatype datatype,
  int source, int tag, MPI_Comm comm, MPI_Request *request );
int stub_MPI_Isend ( void *buf, int count, MPI_Datatype datatype,
  int dest, int tag, MPI_Request *request, MPI_Comm comm );
int stub_MPI_Recv ( void *buf, int count, MPI_Datatype datatype,
  int source, int tag, MPI_Comm comm, MPI_Status *status );
int stub_MPI_Reduce ( void *data1, void *data2, int n, MPI_Datatype datatype,
  MPI_Op operation, int receiver, MPI_Comm comm );
int mpi_reduce_double ( double *data1, double *data2, int n,
  MPI_Op operation );
int mpi_reduce_float ( float *data1, float *data2, int n,
  MPI_Op operation );
int mpi_reduce_int ( int *data1, int *data2, int n,
  MPI_Op operation );
int stub_MPI_Reduce_scatter ( void *sendbuf, void *recvbuf, int recvcounts,
  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm );
int stub_MPI_Rsend ( void *data, int n, MPI_Datatype datatype, int iproc,
  int itag, MPI_Comm comm );
int stub_MPI_Send ( void *buf, int count, MPI_Datatype datatype,
  int dest, int tag, MPI_Comm comm );
int stub_MPI_Wait ( MPI_Request *request, MPI_Status *status );
int stub_MPI_Waitall ( int icount, int irequest, MPI_Status status );
int stub_MPI_Waitany ( int count, MPI_Request *request, int *index,
  MPI_Status *status );

#define MPI_Allgather( sbuf, scnt, stype, rbuf, rcnt, rtype, comm )          \
  stub_MPI_Allgather( sbuf, scnt, stype, rbuf, rcnt, rtype, comm )
#define MPI_Allgatherv( sbuf, scnt, stype, rbuf, rcnt, displs, rtype, comm ) \
  stub_MPI_Allgatherv( sbuf, scnt, stype, rbuf, rcnt, displs, rtype, comm )
#define MPI_Allreduce( sbuf, rbuf, cnt, dt, op, comm )                       \
  stub_MPI_Allreduce( sbuf, rbuf, cnt, dt, op, comm )
#define MPI_Barrier( comm ) stub_MPI_Barrier( comm )
#define MPI_Bcast( data, n, dt, node, comm )                                 \
  stub_MPI_Bcast( data, n, dt, node, comm )
#define MPI_Irecv( buf, cnt, dt, src, tag, comm, req )                       \
  stub_MPI_Irecv( buf, cnt, dt, src, tag, comm, req )
#define MPI_Isend( buf, cnt, dt, dest, tag, req, comm )                      \
  stub_MPI_Isend( buf, cnt, dt, dest, tag, req, comm )
#define MPI_Recv(buf, cnt, dt, src, tag, comm, st)                           \
  stub_MPI_Recv(buf, cnt, dt, src, tag, comm, st)
#define MPI_Reduce( data1, data2, n, dt, op, receiver, comm )                \
  stub_MPI_Reduce( data1, data2, n, dt, op, receiver, comm )
#define MPI_Reduce_scatter( sbuf, rbuf, rcnt, dt, op, comm )                 \
  stub_MPI_Reduce_scatter( sbuf, rbuf, rcnt, dt, op, comm )
#define MPI_Rsend( data, n, dt, iproc, itag, comm )                          \
  stub_MPI_Rsend( data, n, dt, iproc, itag, comm )
#define MPI_Send(buf, cnt, dt, src, tag, comm)                               \
  stub_MPI_Send(buf, cnt, dt, src, tag, comm)
#define MPI_Wait( req, status )                                              \
  stub_MPI_Wait( req, status )
#define MPI_Waitall( icount, ireq, status )                                  \
  stub_MPI_Waitall( icount, ireq, status )
#define mpi_waitany( cnt, req, index, status )                               \
  stub_MPI_Waitany( cnt, req, index, status )

#endif
