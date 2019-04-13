#include <stdlib.h>

#include "memory.h"

void operator delete(void* ptr, size_t size)
{
    free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
    free(ptr);
}

