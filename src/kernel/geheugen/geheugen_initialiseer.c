#include "geheugen.h"

void geheugen_initialiseer(MemoryInfo *memory_info)
{
    // Hier komt de code om het geheugen te initialiseren
    printk("Geheugen initialisatie gestart...\n");
    printk("De kernel begint op %lx en eindigd op %lx en beslaat dus %lx \n",_KernelStart, _KernelEnd, (_KernelEnd - _KernelStart));
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        if(desc->Type == MEMORY_TYPE_CONVENTIONAL_MEMORY && desc->PhysicalStart >= (uint64_t)_KernelEnd)
        {
            if(allocatie_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_ALLOCATION)
            {
                allocatie_geheugen_blok = desc;
                printk("Allocatie geheugen blok gevonden op %lx met een grootte van %lx\n", desc->PhysicalStart, desc->NumberOfPages * MEMORY_PAGE_SIZE);
            }
            else if(paging_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_PAGING) // minimaal 2MB voor paging
            {
                paging_geheugen_blok = desc;
                printk("Paging geheugen blok gevonden op %lx met een grootte van %lx\n", desc->PhysicalStart, desc->NumberOfPages * MEMORY_PAGE_SIZE);
            }
        }
        else if(desc->Type != MEMORY_TYPE_CONVENTIONAL_MEMORY)
        {
            geheugen_kaart_debug(desc);
        }
    }
    printk("Geheugen initialisatie voltooid.\n");
}