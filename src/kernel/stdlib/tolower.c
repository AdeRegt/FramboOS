#include "lualib.h"

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        // Tel 32 op (of doe een bitwise OR met 0x20)
        return c + ('a' - 'A');
    }
    return c;
}