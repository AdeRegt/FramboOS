#include "geheugen.h"

void* memset(void* s, int c, uint64_t n) {
    uint8_t* p = s;
    while(n--) *p++ = (uint8_t)c;
    return s;
}