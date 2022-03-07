#include "utility.h"

void swap_int(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_size(size_t* a, size_t* b)
{
    size_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_ptr(void** a, void** b)
{
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}
