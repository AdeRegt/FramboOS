#include "geheugen.h"

long long kernel_atoll(const char* s) {
    long long res = 0; int sign = 1;
    while(*s == ' ' || *s == '\t') s++;
    if (*s == '-') { sign = -1; s++; }
    while(*s >= '0' && *s <= '9') res = res * 10 + (*s++ - '0');
    return res * sign;
}
