#include "geheugen.h"

void init_allocator(MemoryDescriptor *desc) {
    // We gebruiken de VirtualStart van de descriptor
    free_list_start = (MemoryBlock *)desc->VirtualStart;
    free_list_start->size = (desc->NumberOfPages * 4096) - sizeof(MemoryBlock);
    free_list_start->free = 1;
    free_list_start->next = NULL;
}