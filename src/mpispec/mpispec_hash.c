#include <stdlib.h>
#include "mpispec_basic.h"
#include "mpispec_error.h"
#include "mpispec_error_function.h"
#include "mpispec_hash.h"

hash_t *MPISpec_Hash() {
    static hash_t *hash = NULL;
    if (hash != NULL) return hash;
    if (MPISpec_Error()) return NULL;
    if ((hash = hash_new()) == NULL) {
        MPISpec_Set_Error_Fun(MPISpec_Alloc_Error);
        MPISpec_Finalize();
        exit(1);
    }
    return hash;
}

void MPISpec_Hash_Del() {
    hash_t *hash = MPISpec_Hash();
    if (hash) hash_free(hash);
}
