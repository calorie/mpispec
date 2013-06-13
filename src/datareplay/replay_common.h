
/* Time-stamp: <2007-11-14 22:35:39 shinya> */

#ifndef REPLAY_COMMON_H
#define REPLAY_COMMON_H

#include <mpi.h>

void
datareplay_open(char *filename);

void
datareplay_replay(void* buf, size_t byte, int count);

void
datareplay_close();

#endif
