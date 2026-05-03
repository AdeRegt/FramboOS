#include "lualib.h"

size_t strspn(const char *s, const char *accept) {
    size_t count = 0;
    
    // Loop door de te controleren string
    for (const char *p = s; *p != '\0'; p++) {
        const char *a;
        
        // Controleer of het huidige karakter in de 'accept' set zit
        for (a = accept; *a != '\0'; a++) {
            if (*p == *a) {
                break; // Gevonden in accept set
            }
        }
        
        // Als we de 'accept' string volledig hebben doorlopen zonder match
        if (*a == '\0') {
            return count;
        }
        
        count++;
    }
    
    return count;
}