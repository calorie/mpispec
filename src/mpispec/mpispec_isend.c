#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mpi.h>
#include "parson/parson.h"
#include "http-get/http-get.h"

#define URL_MAX_LENGTH 128

int MPI_Isend(void *buf, int count, MPI_Datatype type, int dest,
              int tag, MPI_Comm comm, MPI_Request *request)
{
    int rank;
    char url[URL_MAX_LENGTH];
    http_get_response_t *res;
    char *json = NULL;
    JSON_Value *root_value;
    JSON_Object *root;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sprintf(url, "http://localhost:4567/isend/set?from=%d&to=%d&tag=%d&status=suspend", rank, dest, tag);
    res = http_get(url);
    if (res->status != 200) {
        printf("[ERROR] API server not found.\n");
        goto leave;
    }

    sprintf(url, "http://localhost:4567/irecv/get?from=%d&to=%d&tag=%d", rank, dest, tag);
    while (1) {
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
        if (strcmp(json_object_get_string(root, "status"), "suspend") == 0) break;
        usleep(100000);
    }
    json_value_free(root_value);

    sprintf(url, "http://localhost:4567/isend/set?from=%d&to=%d&tag=%d&status=none", rank, dest, tag);
    res = http_get(url);
    if (res->status != 200) {
        printf("[ERROR] API server not found.\n");
        goto leave;
    }

leave:
    if (res) http_get_free(res);
    if (json) free(json);
    return PMPI_Isend(buf, count, type, dest, tag, comm, request);
}
