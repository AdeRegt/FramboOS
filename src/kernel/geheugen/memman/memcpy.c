#include "geheugen.h"

void memcpy(void* dest, const void* src, uint64_t n)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for(uint64_t i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
}