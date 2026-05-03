#include "lualib.h"

// Dit is een placeholder voor jouw hardware-specifieke functie
// In x86_64 lees je dit meestal uit via poort 0x70 en 0x71 (CMOS)
extern time_t platform_get_unix_timestamp();

time_t time(time_t *tloc) {
    time_t t = platform_get_unix_timestamp();

    if (tloc) {
        *tloc = t;
    }

    return t;
}