
/* Time-stamp: <2007-11-14 22:35:16 shinya> */

#ifndef LOG_COMMON_H
#define LOG_COMMON_H

#include <mpi.h>

void
datalog_open(char *filename);

void
datalog_log(void* buf, size_t byte, int count);

void
datalog_close();

#endif
