#include "geheugen.h"

void define_page_memory_range_from_memory_descriptor(MemoryDescriptor *desc){
    const uint64_t LARGE_PAGE_SIZE = 2 * 1024 * 1024; // 2 MiB
    uint64_t region_start = desc->PhysicalStart;
    uint64_t region_size = desc->NumberOfPages * MEMORY_PAGE_SIZE;
    uint64_t region_end = region_start + region_size;

    // Align start to next 2 MiB boundary if needed
    uint64_t aligned_start = (region_start + LARGE_PAGE_SIZE - 1) & ~(LARGE_PAGE_SIZE - 1);

    for(uint64_t addr = aligned_start; addr + LARGE_PAGE_SIZE <= region_end; addr += LARGE_PAGE_SIZE){
        map_memory(master_page_table, (void*)addr, (void*)addr);
    }
}