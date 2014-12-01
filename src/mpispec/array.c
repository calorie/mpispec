#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "array.h"

#define N 10

array_t* array_new(size_t element_size) {
    if (element_size == 0) return NULL;

    array_t* array;
    array = malloc(sizeof(array_t));
    if (array == NULL) return NULL;

    array->element_size = element_size;
    array->size = 0;
    array->capacity = 0;
    array->data = NULL;

    return array;
}

void array_delete(array_t** const array) {
    if ((array == NULL) || (*array == NULL)) return;

    void* p;

    (*array)->element_size = 0;
    (*array)->size = 0;
    (*array)->capacity = 0;

    if (NULL != (*array)->data) {
        free((*array)->data);
        (*array)->data = NULL;
    }

    p = *array;
    free(p);
    *array = NULL;
}

int array_add(array_t* const array, const void* const data) {
    if ((array == NULL) || (data == NULL)) return 1;

    if ((array->size % N) == 0) {
        size_t new_size = (array->size + N) * array->element_size;
        char* p = realloc(array->data, new_size);
        if (p == NULL) return -1;
        array->data = p;
        array->capacity = new_size;
    }
    assert((array->size + 1) * array->element_size < array->capacity);
    memcpy(array->data + array->size * array->element_size, data,
           array->element_size);
    ++array->size;

    return 0;
}

void* array_get_element(array_t* const array, size_t idx) {
    if ((array == NULL) || (array->size <= idx) || (array->data == NULL))
        return NULL;
    return array->data + idx * array->element_size;
}
