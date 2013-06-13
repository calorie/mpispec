
/* Time-stamp: <2007-11-14 22:35:35 shinya> */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <replay_common.h>

static FILE *logfile;

void
datareplay_open(char *filename)
{
  if(NULL == (logfile = fopen(filename, "rb"))) {
    printf("Error: Can't open %s\n", filename);
    exit(-1);
  }
}

void
datareplay_replay(void* buf, size_t byte, int count)
{
  fread(buf, byte, count, logfile);
}

void
datareplay_close()
{
  fclose(logfile);
}
