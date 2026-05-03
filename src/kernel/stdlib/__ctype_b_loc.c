#include "lualib.h"
/* ctype.h of een interne header */
enum {
    _MYOS_UPPER  = 1 << 0,
    _MYOS_LOWER  = 1 << 1,
    _MYOS_ALPHA  = 1 << 2,
    _MYOS_DIGIT  = 1 << 3,
    _MYOS_XDIGIT = 1 << 4,
    _MYOS_SPACE  = 1 << 5,
    _MYOS_PRINT  = 1 << 6,
    _MYOS_GRAPH  = 1 << 7,
    _MYOS_BLANK  = 1 << 8,
    _MYOS_CNTRL  = 1 << 9,
    _MYOS_PUNCT  = 1 << 10,
    _MYOS_ALNUM  = 1 << 11
};

// De tabel met de juiste bitmaskers
static const unsigned short _ctype_b_data[256] = {
    ['A'] = _MYOS_UPPER | _MYOS_ALPHA | _MYOS_PRINT | _MYOS_XDIGIT,
    ['B'] = _MYOS_UPPER | _MYOS_ALPHA | _MYOS_PRINT | _MYOS_XDIGIT,
    // ... vul dit aan voor de rest ...
    [' '] = _MYOS_SPACE | _MYOS_BLANK | _MYOS_PRINT,
    ['0'] = _MYOS_DIGIT | _MYOS_ALNUM | _MYOS_PRINT | _MYOS_XDIGIT,
};

const unsigned short *__ctype_b_ptr = _ctype_b_data;

const unsigned short **__ctype_b_loc(void) {
    return &__ctype_b_ptr;
}