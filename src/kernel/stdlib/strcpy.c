#include "lualib.h"

char *strcpy(char *dest, const char *src) {
    char *saved_dest = dest;

    // Kopieer karakters tot en met de null-terminator
    while ((*dest++ = *src++)) {
        // De conditie kopieert de byte en stopt als de byte '\0' is
    }

    return saved_dest;
}