#include "lualib.h"

char *strchr(const char *s, int c) {
    char target = (char)c;

    // Doorloop de string inclusief de null-terminator
    while (1) {
        if (*s == target) {
            // Gevonden! Cast naar (char *) om de const weg te halen
            return (char *)s;
        }
        if (*s == '\0') {
            // Einde bereikt en niets gevonden
            break;
        }
        s++;
    }

    return NULL;
}