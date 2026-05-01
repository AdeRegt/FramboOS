#include "geheugen.h"

char* strstr(const char* h, const char* n) {
    size_t len = strlen(n);
    if (!len) return (char*)h;
    while(*h) { if (!memcmp(h, n, len)) return (char*)h; h++; }
    return NULL;
}