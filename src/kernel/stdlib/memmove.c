#include "lualib.h"

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s || n == 0) {
        return dest;
    }

    if (d < s) {
        // Geen overlap of dest ligt vóór src: kopieer vooruit
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        // Dest ligt na src: kopieer achteruit om data-corruptie te voorkomen
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}