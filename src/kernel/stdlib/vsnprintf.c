#include "lualib.h"

static void long_to_string(char *buf, unsigned long long n, int base) {
    char tmp[64];
    int i = 0;
    if (n == 0) { tmp[i++] = '0'; }
    else {
        while (n > 0) {
            int rem = n % base;
            tmp[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
            n /= base;
        }
    }
    while (i > 0) { *buf++ = tmp[--i]; }
    *buf = '\0';
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    size_t written = 0;
    
    for (const char *f = format; *f != '\0'; f++) {
        if (*f == '%' && *(f + 1) != '\0') {
            f++;
            if (*f == 's') {
                char *s = va_arg(ap, char *);
                while (*s && written < size - 1) {
                    str[written++] = *s++;
                }
            } else if (*f == 'd') {
                long long d = va_arg(ap, long long);
                char buf[64];
                if (d < 0) {
                    if (written < size - 1) str[written++] = '-';
                    d = -d;
                }
                long_to_string(buf, d, 10);
                for (int i = 0; buf[i] && written < size - 1; i++) str[written++] = buf[i];
            } else if (*f == 'x' || *f == 'p') {
                unsigned long long x = va_arg(ap, unsigned long long);
                char buf[64];
                long_to_string(buf, x, 16);
                for (int i = 0; buf[i] && written < size - 1; i++) str[written++] = buf[i];
            }
            // Voeg hier meer types toe indien nodig (%c, %u)
        } else {
            if (written < size - 1) {
                str[written++] = *f;
            }
        }
    }

    if (size > 0) {
        str[written] = '\0';
    }
    return (int)written;
}