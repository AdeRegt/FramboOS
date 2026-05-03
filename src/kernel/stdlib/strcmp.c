#include "lualib.h"

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    // Geef het verschil terug tussen de eerste karakters die verschillen
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}