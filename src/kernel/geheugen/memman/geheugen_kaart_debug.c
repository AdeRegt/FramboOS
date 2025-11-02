#include "geheugen.h"

void geheugen_kaart_debug(MemoryDescriptor *desc)
{
    printk("Geheugenblok gevonden van type %s, beginnend op %lx met een grootte van %lx\n", geheugen_geheugenblok_type_naar_string(desc->Type), desc->PhysicalStart, desc->NumberOfPages * MEMORY_PAGE_SIZE);
}