
/* Time-stamp: <2007-11-14 22:35:54 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <req_table.h>

#define REQ_TABLE_SIZE 1000

static FILE* logfile;
static ReqTable req_table[REQ_TABLE_SIZE];

void
initialize_req_table()
{
  int i;

  for(i = 0; i < REQ_TABLE_SIZE; i++) {
    req_table[i].req = NULL;
  }
}

void*
register_req_table(MPI_Request *req, void *buf, int count, int size)
{
  int i;

  for(i = 0; i < REQ_TABLE_SIZE; i++) {
    if(req_table[i].req == NULL) {
      req_table[i].req = req;
      req_table[i].buf = buf;
      req_table[i].count = count;
      req_table[i].size = size;
      break;
    }
  }
}

void*
search_req_table(MPI_Request *req, void **buf, int *count, int *size)
{
  int i;

  for(i = 0; i < REQ_TABLE_SIZE; i++) {
    if(req_table[i].req == req) {
      *buf = req_table[i].buf;
      *count = req_table[i].count;
      *size = req_table[i].size;
      break;
    }
  }
}

void*
delete_req_table(MPI_Request *req, void **buf, int *count, int *size)
{
  int i;

  for(i = 0; i < REQ_TABLE_SIZE; i++) {
    if(req_table[i].req == req) {
      *buf = req_table[i].buf;
      *count = req_table[i].count;
      *size = req_table[i].size;
      req_table[i].req = NULL;
      break;
    }
  }
}
