#include "geheugen.h"

uint32_t get_lapic_id()
{
    void* lapic_base = (void*)LAPIC_BASE;
    volatile uint32_t* lapic = (uint32_t*)lapic_base;

    uint32_t lapic_id = lapic[0x20 / 4];
    lapic_id >>= 24;
    return lapic_id;
}