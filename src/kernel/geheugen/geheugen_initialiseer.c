#include "geheugen.h"

void geheugen_initialiseer(MemoryInfo *memory_info)
{
    // Hier komt de code om het geheugen te initialiseren
    printk("Geheugen initialisatie gestart...\n");
    allocatie_geheugen_blok = NULL;
    paging_geheugen_blok = NULL;
    memory_info_pointer = memory_info;
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        desc->VirtualStart = desc->PhysicalStart; // Identity mapping instellen
        if(desc->Type == MEMORY_TYPE_CONVENTIONAL_MEMORY && desc->PhysicalStart >= (uint64_t)_KernelEnd)
        {
            if(allocatie_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_ALLOCATION)
            {
                allocatie_geheugen_blok = desc;
                printk("Allocatie geheugen blok gevonden op %lx met grootte %lx pagina's\n", desc->PhysicalStart, desc->NumberOfPages);
            }
            else if(paging_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_PAGING) // minimaal 2MB voor paging
            {
                paging_geheugen_blok = desc;
                printk("Paging geheugen blok gevonden op %lx met grootte %lx pagina's\n", desc->PhysicalStart, desc->NumberOfPages);
            }
        }
    }
    kernel_geheugen_blok = geheugenblok_van_address((uint64_t)geheugen_initialiseer);
    if(allocatie_geheugen_blok == NULL || paging_geheugen_blok == NULL || kernel_geheugen_blok == NULL)
    {
        printk("Niet genoeg geheugen gevonden voor allocatie of paging of kernel!\n");for(;;);
    }
    printk("De kernel begint op %lx en eindigd op %lx en beslaat dus %lx \n",_KernelStart, _KernelEnd, (_KernelEnd - _KernelStart));
    master_page_table = alloc_page();
    // gewoon om zeker te zijn het begin te mappen...
    for(uint64_t valve = 0 ; valve < (0xFFFFF000/PAGE_GAP_SIZE) ; valve++){
        define_linear_memory_block((void*)(valve*PAGE_GAP_SIZE));
    }
    // en nu de precieze mapping volgens de memory descriptors
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        if(desc->Type != MEMORY_TYPE_CONVENTIONAL_MEMORY)
        {
            define_page_memory_range_from_memory_descriptor(desc);
        }
    }
    define_page_memory_range_from_memory_descriptor(allocatie_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(paging_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(kernel_geheugen_blok);
    asm volatile ("mov %0, %%cr3" : : "r" (master_page_table));
}