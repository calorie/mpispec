#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "parson/parson.h"
#include "http-get/http-get.h"

#define URL_MAX_LENGTH 128

int MPI_Send(void *buf, int count, MPI_Datatype type, int dest,
             int tag, MPI_Comm comm)
{
    int rank, cache = 0;
    char url[URL_MAX_LENGTH];
    http_get_response_t *res;
    char *json = NULL;
    JSON_Value *root_value;
    JSON_Object *root;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sprintf(url, "http://localhost:4567/send/set?from=%d&to=%d&tag=%d&data=%d", rank, dest, tag, *((int *)buf));
    res = http_get(url);
    if (res->status != 200) {
        printf("[ERROR] API server not found.\n");
        goto leave;
    }

    json = calloc(res->size + 1, 1);
    if (!json) {
        printf("[ERROR] JSON response not found.\n");
        goto leave;
    }
    strncpy(json, res->data, res->size);

    root_value = json_parse_string(json);
    root = json_value_get_object(root_value);
    if (json_object_get_boolean(root, "is_exist") == 1) cache = 1;
    json_value_free(root_value);

leave:
    if (res) http_get_free(res);
    if (json) free(json);
    return (cache == 1) ? MPI_SUCCESS : PMPI_Send(buf, count, type, dest, tag, comm);
}
