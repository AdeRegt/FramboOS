#include "geheugen.h"

void *memset(void *s, char val, uint64_t count)
{
	uint64_t _count = count;
    unsigned char *tmp = (unsigned char *)s;
    for(; _count != 0; _count--) *tmp++ = val;
    return s;

}