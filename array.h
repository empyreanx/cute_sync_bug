#ifndef WS_ARRAY_H
#define WS_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    size_t capacity;
    size_t value_size;
    size_t size;
    void* values;
} array_t;

typedef int (*array_compare_fn)(const void* value1, const void* value2);

array_t* array_new(size_t capacity, size_t value_size);
void array_free(array_t* array);
size_t array_size(const array_t* array);
void* array_values(array_t* array);
bool array_equal(array_t* array1, array_t* array2);
void array_copy(array_t* dst_array, array_t* src_array);
void array_swap(array_t* array1, array_t* array2);
void array_resize(array_t* array, size_t size);
void array_push(array_t* array, void* value);
void array_pop(array_t* array, void* value);
void* array_get(array_t* array, size_t index);
void* array_back(array_t* array);
void array_insert(array_t* array, size_t index, void* value);
void array_remove(array_t* array, size_t index);
void array_swap_remove(array_t* array, size_t index);
void array_sort(array_t* array, array_compare_fn compare);

#endif // WS_ARRAY_H

