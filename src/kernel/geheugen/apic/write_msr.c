#include "geheugen.h"

void write_msr(uint32_t msr, uint64_t value) {
    uint32_t low = (uint32_t)value;
    uint32_t high = (uint32_t)(value >> 32);
    __asm__ volatile ("wrmsr" :: "c"(msr), "a"(low), "d"(high));
}