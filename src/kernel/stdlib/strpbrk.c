#include "lualib.h"

char *strpbrk(const char *s, const char *accept) {
    if (!s || !accept) {
        return NULL;
    }

    while (*s != '\0') {
        const char *a = accept;
        while (*a != '\0') {
            if (*s == *a) {
                // Gevonden! Geef de pointer in s terug.
                return (char *)s;
            }
            a++;
        }
        s++;
    }

    // Geen van de karakters uit 'accept' gevonden in 's'
    return NULL;
}