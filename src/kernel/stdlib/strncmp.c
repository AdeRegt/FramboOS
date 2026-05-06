#include "lualib.h"

int strncmp(const char *s1, const char *s2, size_t n) {
    // Als n 0 is, zijn de strings per definitie "gelijk" over 0 karakters
    if (n == 0) {
        return 0;
    }

    // Loop zolang de karakters gelijk zijn en we n niet hebben bereikt
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    // Als we n hebben bereikt voordat er een verschil was
    if (n == 0) {
        return 0;
    }

    // Retourneer het verschil tussen de karakters
    // Cast naar unsigned char om sign-extension problemen te voorkomen
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
