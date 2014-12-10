#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include "mpispec_hash.h"
#include "mpispec_stub.h"

void MPISpec_Stub(void *data, int from, int to, int tag) {
    char key[MPISPEC_HASH_KEY_MAX];
    hash_t *hash = MPISpec_Hash();
    sprintf(key, "%d_%d_%d", from, to, tag);
    hash_set(hash, strdup(key), data);
}
