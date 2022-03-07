#include "array.h"
#include "utility.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool mem_equal(void* ptr1, void* ptr2, size_t size)
{
    return 0 == memcmp(ptr1, ptr2, size);
}

array_t* array_new(size_t capacity, size_t value_size)
{
    assert(capacity > 0);
    assert(value_size > 0);

    array_t* array = malloc(sizeof(array_t));

    array->capacity = capacity;
    array->size = 0;
    array->value_size = value_size;
    array->values = malloc(capacity * value_size);

    return array;
}

void array_free(array_t* array)
{
    assert(array->values);
    assert(array);
    free(array->values);
    free(array);
}

size_t array_size(const array_t* array)
{
    assert(array);
    return array->size;
}

void* array_values(array_t* array)
{
    assert(array);
    return array->values;
}

bool array_equal(array_t* array1, array_t* array2)
{
    assert(array1);
    assert(array2);

    size_t size = array1->size;
    size_t value_size = array1->value_size;

    return size == array2->size &&
           value_size == array2->value_size &&
           mem_equal(array1->values, array2->values, size * value_size);
}

void array_copy(array_t* dst_array, array_t* src_array)
{
    assert(dst_array);
    assert(src_array);

    size_t size = src_array->size;
    array_resize(dst_array, size);
    memcpy(dst_array->values, src_array->values, size * src_array->value_size);
}

void array_swap(array_t* array1, array_t* array2)
{
    assert(array1);
    assert(array2);

    swap_size(&array1->capacity, &array2->capacity);
    swap_size(&array1->value_size, &array2->value_size);
    swap_size(&array1->size, &array2->size);
    swap_ptr(&array1->values, &array2->values);
}

void array_resize(array_t* array, size_t new_size)
{
    assert(array);

    if (new_size > array->capacity)
    {
        while (array->capacity < new_size)
        {
            array->capacity += (array->capacity + 1) >> 1;
        }

        array->values = realloc(array->values, array->capacity * array->value_size);
    }

    array->size = new_size;
}

void array_push(array_t* array, void* value)
{
    assert(array);
    assert(value);

    size_t size = array->size;
    array_resize(array, size + 1);
    memcpy(array_get(array, size), value, array->value_size);
}

void array_pop(array_t* array, void* value)
{
    assert(array);
    assert(array->size > 0);

    size_t size = array->size;

    if (value)
        memcpy(value, array_get(array, size - 1), array->value_size);

    array_resize(array, size - 1);
}

void* array_get(array_t* array, size_t index)
{
    assert(array);
    assert(index < array->size);

    return (char*)array->values + (index * array->value_size);
}

void* array_back(array_t* array)
{
    assert(array);
    assert(array->size > 0);

    return array_get(array, array->size - 1);
}


void array_insert(array_t* array, size_t index, void* value)
{
    assert(array);
    assert(index < array->size);
    assert(value);

    size_t size = array->size;
    array_resize(array, size + 1);

    for (size_t i = size; i > index; i--)
    {
        memcpy(array_get(array, i), array_get(array, i - 1), array->value_size);
    }

    memcpy(array_get(array, index), value, array->value_size);;
}

void array_remove(array_t* array, size_t index)
{
    assert(array);
    assert(index < array->size);

    size_t size = array->size;

    for (size_t i = index; i < size - 1; i++)
    {
        memcpy(array_get(array, i), array_get(array, i + 1), array->value_size);
    }

    array_resize(array, size - 1);
}

void array_swap_remove(array_t* array, size_t index)
{
    assert(array);
    assert(index < array->size);

    size_t size = array->size;
    memcpy(array_get(array, index), array_get(array, size - 1), array->value_size);
    array_resize(array, size - 1);
}

void array_sort(array_t* array, array_compare_fn compare)
{
    assert(array);
    assert(compare);
    qsort(array->values, array->size, array->value_size, compare);
}

