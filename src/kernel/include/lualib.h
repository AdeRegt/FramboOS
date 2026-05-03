#include <stddef.h>
#include <stdarg.h>

/**
 * Berekent de lengte van de string s, 
 * exclusief het afsluitende nul-karakter ('\0').
 */
size_t strlen(const char *s);

/**
 * Vergelijkt twee strings op basis van de huidige locale.
 * Geeft <0, 0, of >0 terug als s1 respectievelijk kleiner, 
 * gelijk of groter is dan s2.
 */
int strcoll(const char *s1, const char *s2);

int strcmp(const char *s1, const char *s2);

/**
 * Vergelijkt de eerste n bytes van de geheugenblokken s1 en s2.
 * Retourneert:
 *   < 0 : s1 is kleiner dan s2
 *     0 : s1 is gelijk aan s2
 *   > 0 : s1 is groter dan s2
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * Kopieert n bytes van src naar dest. De geheugengebieden mogen overlappen.
 * Retourneert de waarde van dest.
 */
void *memmove(void *dest, const void *src, size_t n);

typedef long long time_t;

struct tm {
    int tm_sec;   // 0-60
    int tm_min;   // 0-59
    int tm_hour;  // 0-23
    int tm_mday;  // 1-31
    int tm_mon;   // 0-11 (let op: januari = 0)
    int tm_year;  // Jaren sinds 1900
    int tm_wday;  // Dagen sinds zondag (0-6)
    int tm_yday;  // Dagen sinds 1 januari (0-365)
    int tm_isdst; // Daylight Saving Time vlag
};

time_t time(time_t *tloc);
long long difftime(time_t time1, time_t time0);

typedef long clock_t;

#define CLOCKS_PER_SEC 1000000 // Bijvoorbeeld voor microseconde precisie

clock_t clock(void);

/**
 * Zoekt naar de eerste verschijning van het karakter c (geconverteerd naar char)
 * in de string s. De afsluitende '\0' wordt beschouwd als onderdeel van de string.
 * Retourneert een pointer naar het karakter, of NULL als het niet wordt gevonden.
 */
char *strchr(const char *s, int c);

/**
 * Formateert een string en slaat deze op in 'str'.
 * Schrijft maximaal 'size' bytes (inclusief de null-terminator).
 * Retourneert het aantal karakters dat geschreven zou zijn als de buffer groot genoeg was.
 */
int snprintf(char *str, size_t size, const char *format, ...);

/**
 * De 'v' variant die een va_list accepteert (nodig voor de interne logica).
 */
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

/**
 * Berekent de lengte van het beginsegment van s dat volledig bestaat
 * uit karakters die voorkomen in de string accept.
 */
size_t strspn(const char *s, const char *accept);

/**
 * Kopieert de string 'src' (inclusief de null-terminator) naar 'dest'.
 * De buffer 'dest' moet groot genoeg zijn.
 * Retourneert de pointer naar 'dest'.
 */
char *strcpy(char *dest, const char *src);

/**
 * Zoekt in string s naar de eerste verschijning van een van de
 * karakters in de string accept.
 * Retourneert een pointer naar het gevonden karakter in s,
 * of NULL als er geen match is gevonden.
 */
char *strpbrk(const char *s, const char *accept);

/**
 * Converteert de string 'nptr' naar een long long integer.
 * 'endptr' wordt gezet op het eerste karakter na het getal.
 * 'base' is het talstelsel (2 tot 36, of 0 voor automatische detectie).
 */
long long strtoll(const char *nptr, char **endptr, int base);
#ifndef _SETJMP_H
#define _SETJMP_H

// Een array van 8 long longs is genoeg voor x86_64
typedef unsigned long long jmp_buf[8];

extern int _setjmp(jmp_buf env);
extern void _longjmp(jmp_buf env, int val);

// Lua zoekt vaak naar setjmp/longjmp zonder underscore
#define setjmp(env) _setjmp(env)
#define longjmp(env, val) _longjmp(env, val)

#endif

/**
 * Veroorzaakt een abnormale programma-beëindiging.
 * Deze functie keert nooit terug naar de aanroeper.
 */
void abort(void) __attribute__((__noreturn__));


/**
 * Retourneert de absolute waarde van een integer.
 */
int abs(int j);

/**
 * Retourneert de absolute waarde van een long integer.
 */
long labs(long j);

/**
 * Retourneert de absolute waarde van een long long integer (64-bit).
 */
long long llabs(long long j);
