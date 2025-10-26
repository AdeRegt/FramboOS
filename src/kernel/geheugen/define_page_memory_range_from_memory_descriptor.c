#include "geheugen.h"

void define_page_memory_range_from_memory_descriptor(MemoryDescriptor *desc){
    uint64_t physical_start = desc->PhysicalStart;
    uint64_t number_of_pages = (( desc->NumberOfPages * MEMORY_PAGE_SIZE ) / PAGE_GAP_SIZE)+1;

    for(uint64_t page = 0; page < number_of_pages; page++){
        uint64_t physical_address = physical_start + (page * PAGE_GAP_SIZE);
        uint64_t virtual_address = physical_address; // Identity mapping

        map_memory(master_page_table, (void*)virtual_address, (void*)physical_address);
    }
}