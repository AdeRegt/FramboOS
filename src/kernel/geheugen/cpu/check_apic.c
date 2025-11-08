#include "geheugen.h"

uint8_t check_apic()
{
    uint32_t a, b, c, d;
    cpuid(1, &a, &b, &c, &d);
    return (d & (1 << 9))!=0;
}