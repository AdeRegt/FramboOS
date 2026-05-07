#include "geheugen.h"

void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size);
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    MemoryBlock *block = (MemoryBlock *)((uint8_t *)ptr - sizeof(MemoryBlock));
    
    // Casus 1: Huidige blok is al groot genoeg
    if (block->size >= size) {
        return ptr;
    }

    // Casus 2: Nieuw blok nodig
    void *new_ptr = malloc(size);
    if (new_ptr) {
        // Kopieer data (gebruik je eigen memcpy)
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
    }
    return new_ptr;
}