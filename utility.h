#ifndef WS_UTILITY_H
#define WS_UTILITY_H

#include <stddef.h>

void swap_int(int* a, int* b);
void swap_size(size_t* a, size_t* b);
void swap_ptr(void** a, void** b);

#define static_arg(type, ...) (&(type){ __VA_ARGS__ })

#endif // WS_UTILITY_H
