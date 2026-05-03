#include "lualib.h"

size_t strlen(const char *s) {
    size_t length = 0;
    
    // Loop door de string tot de null-terminator is bereikt
    while (s[length] != '\0') {
        length++;
    }
    
    return length;
}