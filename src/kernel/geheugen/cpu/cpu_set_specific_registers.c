#include "geheugen.h"

void cpu_set_specific_registers(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}