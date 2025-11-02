#include "geheugen.h"

MemoryDescriptor* geheugenblok_van_address(uint64_t address)
{
    for(uint64_t offset = 0; offset < memory_info_pointer->mMapSize; offset += memory_info_pointer->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info_pointer->mMap + offset);
        uint64_t start = desc->PhysicalStart;
        uint64_t end = desc->PhysicalStart + (desc->NumberOfPages * MEMORY_PAGE_SIZE);
        if(address >= start && address < end)
        {
            return desc;
        }
    }
    return NULL;
}