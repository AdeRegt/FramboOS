#include "geheugen.h"

void idt_set_entry(IDTDescEntry *entry, void (*handler)(), uint16_t selector, uint8_t ist, uint8_t type_attr)
{
    uint64_t addr = (uint64_t)handler;

    entry->offset0 = (uint16_t)(addr & 0xFFFF);
    entry->selector = selector;
    entry->ist = ist & 0x7;            // only lower 3 bits are valid for IST
    entry->type_attr = type_attr;
    entry->offset1 = (uint16_t)((addr >> 16) & 0xFFFF);
    entry->offset2 = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    entry->ignore = 0;                 // reserved, should be zero
}