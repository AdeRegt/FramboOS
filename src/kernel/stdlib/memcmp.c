#include "lualib.h"

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            // Retourneer het verschil tussen de eerste afwijkende bytes
            return p1[i] - p2[i];
        }
    }

    // Geen verschillen gevonden binnen n bytes
    return 0;
}