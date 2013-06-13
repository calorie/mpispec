
/* Time-stamp: <2007-11-14 22:35:57 shinya> */

#ifndef REQ_TABLE_H
#define REQ_TABLE_H

#include <mpi.h>

typedef struct {
  MPI_Request *req;
  void *buf;
  int count;
  int size;
} ReqTable;

void
initialize_req_table();

void*
register_req_table(MPI_Request *req, void *buf, int count, int size);

void*
search_req_table(MPI_Request *req, void **buf, int *count, int *size);

void*
delete_req_table(MPI_Request *req, void **buf, int *count, int *size);

#endif
