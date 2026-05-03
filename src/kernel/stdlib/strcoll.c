#include "lualib.h"

int strcoll(const char *s1, const char *s2) {
    // In een basis OS-omgeving is er meestal geen locale-ondersteuning.
    // Daarom vallen we terug op de standaard byte-vergelijking.
    return strcmp(s1, s2);
}