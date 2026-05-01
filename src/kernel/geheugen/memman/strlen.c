#include "geheugen.h"

size_t strlen(const char* s) {
    size_t l = 0; while(s[l]) l++; return l;
}