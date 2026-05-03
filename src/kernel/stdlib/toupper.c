#include "lualib.h"

int toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        // Trek 32 af (of doe een bitwise AND met ~0x20)
        return c - ('a' - 'A'); 
    }
    return c;
}