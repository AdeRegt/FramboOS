#include "lualib.h"
#include <ctype.h> // Voor isspace, isdigit, isalpha

long long strtoll(const char *nptr, char **endptr, int base) {
    const char *s = nptr;
    unsigned long long acc;
    int c;
    unsigned long long cutoff;
    int neg = 0, any, cutlim;

    // 1. Sla witruimte over
    while (isspace((unsigned char)*s)) {
        s++;
    }

    // 2. Behandel het teken (+ of -)
    if (*s == '-') {
        neg = 1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    // 3. Detecteer of bepaal de basis
    if ((base == 0 || base == 16) && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
        s += 2;
        base = 16;
    }
    if (base == 0) {
        base = (*s == '0') ? 8 : 10;
    }

    // 4. Bereken cutoff voor overflow-detectie (optioneel voor basisversie)
    any = 0;
    acc = 0;

    // 5. Converteer de string naar het getal
    for ( ; ; s++) {
        c = (unsigned char)*s;
        if (isdigit(c)) {
            c -= '0';
        } else if (isalpha(c)) {
            c = toupper(c) - 'A' + 10;
        } else {
            break;
        }
        if (c >= base) {
            break;
        }
        
        any = 1;
        acc *= base;
        acc += c;
    }

    // 6. Zet de endptr
    if (endptr != NULL) {
        *endptr = (char *)(any ? s : nptr);
    }

    return (neg ? -(long long)acc : (long long)acc);
}