#include "lualib.h"

char *strstr(const char *haystack, const char *needle) {
    size_t needle_len = strlen(needle);

    // Volgens de standaard: als needle leeg is, retourneer haystack
    if (needle_len == 0) {
        return (char *)haystack;
    }

    // Loop door de haystack
    while (*haystack != '\0') {
        // Controleer of de huidige positie overeenkomt met de needle
        // We gebruiken strncmp om precies needle_len karakters te checken
        if (*haystack == *needle && strncmp(haystack, needle, needle_len) == 0) {
            return (char *)haystack;
        }
        haystack++;
    }

    return NULL;
}