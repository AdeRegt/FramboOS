#include "geheugen.h"

void free(void *ptr) {
    if (!ptr) return;
    MemoryBlock *block = (MemoryBlock *)((uint8_t *)ptr - sizeof(MemoryBlock));
    block->free = 1;
    
    // Optioneel: Scan de lijst en voeg aangrenzende vrije blokken samen
}