#include "mpispec_hash.h"

hash_t *MPISpec_Hash() {
    static hash_t *hash = NULL;
    if (hash != NULL) return hash;
    hash = hash_new();
    return hash;
}

void MPISpec_Hash_Del() { hash_free(MPISpec_Hash()); }
