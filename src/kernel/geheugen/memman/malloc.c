#include "geheugen.h"

void *malloc(size_t size) {
    MemoryBlock *curr = free_list_start;

    while (curr) {
        if (curr->free && curr->size >= size) {
            // Kunnen we dit blok splitsen? (Alleen als er genoeg ruimte is voor een nieuwe header + 1 byte)
            if (curr->size > size + sizeof(MemoryBlock)) {
                MemoryBlock *new_block = (MemoryBlock *)((uint8_t *)curr + sizeof(MemoryBlock) + size);
                new_block->size = curr->size - size - sizeof(MemoryBlock);
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }
            curr->free = 0;
            return (void *)((uint8_t *)curr + sizeof(MemoryBlock));
        }
        curr = curr->next;
    }
    return NULL; // Out of memory
}