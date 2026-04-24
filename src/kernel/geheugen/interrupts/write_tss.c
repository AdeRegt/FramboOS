#include "geheugen.h"

void write_tss(TSSEntry* entry, uint64_t tss_addr) {
    uint32_t limit = sizeof(TSS);
    entry->common.limit_low = limit & 0xFFFF;
    entry->common.base_low = tss_addr & 0xFFFF;
    entry->common.base_middle = (tss_addr >> 16) & 0xFF;
    entry->common.access = 0x89; // Present, Type: 64-bit TSS (Available)
    entry->common.granularity = ((limit >> 16) & 0x0F);
    entry->common.base_high = (tss_addr >> 24) & 0xFF;
    entry->base_upper32 = (tss_addr >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}