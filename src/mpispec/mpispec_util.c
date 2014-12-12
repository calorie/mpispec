#include <mpi.h>
#include <sys/time.h>
#include <time.h>
#include "mpispec_util.h"

int MPISpec_Rank(void) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
}

int MPISpec_Size(void) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;
}

double MPISpec_Start_Time(void) {
    static double start_time;
    if (start_time == 0) start_time = MPISpec_Current_Time();
    return start_time;
}

double MPISpec_Current_Time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}
