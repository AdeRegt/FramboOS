#include "geheugen.h"
#include <stddef.h>

void* memcpy(void* restrict dest, const void* restrict src, size_t n)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for(size_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}