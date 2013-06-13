
/* Time-stamp: <2007-11-14 22:35:08 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <log_common.h>

static FILE *logfile;

void
datalog_open(char *filename)
{
  if(NULL == (logfile = fopen(filename, "wb"))) {
    printf("Logging Error: Can't open %s\n", filename);
    exit(-1);
  }
}

void
datalog_log(void* buf, size_t byte, int count)
{
  fwrite(buf, byte, count, logfile);
}

void
datalog_close()
{
  fclose(logfile);
}
