#include "geheugen.h"
#include <stddef.h>
#include <stdint.h>

/* Simple malloc/free allocator for kernel Lua */

typedef struct {
    size_t size;
    int free;
} AllocationHeader;

#define HEADER_SIZE sizeof(AllocationHeader)

static uint64_t alloc_ptr = 0;
static uint64_t alloc_end = 0;

/* Initialize allocator at first call */
static void init_allocator(void) {
    if (alloc_ptr == 0 && allocatie_geheugen_blok != NULL) {
        alloc_ptr = allocatie_geheugen_blok->PhysicalStart;
        alloc_end = allocatie_geheugen_blok->PhysicalStart + 
                   (allocatie_geheugen_blok->NumberOfPages * MEMORY_PAGE_SIZE);
    }
}

void* malloc(size_t size) {
    if (size == 0) return NULL;
    
    init_allocator();
    
    if (alloc_ptr == 0) {
        return NULL;  /* Allocator not initialized */
    }
    
    /* First-fit: search for free block */
    uint64_t current = alloc_ptr;
    
    while (current < alloc_end) {
        AllocationHeader *hdr = (AllocationHeader*)current;
        
        if (hdr->free && hdr->size >= size) {
            /* Found suitable block, mark as used and return */
            hdr->free = 0;
            return (void*)(current + HEADER_SIZE);
        }
        
        current += HEADER_SIZE + hdr->size;
    }
    
    /* No free block found, allocate new memory if space available */
    if (current + HEADER_SIZE + size <= alloc_end) {
        AllocationHeader *hdr = (AllocationHeader*)current;
        hdr->size = size;
        hdr->free = 0;
        alloc_ptr = current + HEADER_SIZE + size;
        return (void*)(current + HEADER_SIZE);
    }
    
    return NULL;  /* Out of memory */
}

void* calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *ptr = malloc(total);
    if (ptr) {
        /* Zero out memory */
        uint8_t *p = (uint8_t*)ptr;
        for (size_t i = 0; i < total; i++) {
            p[i] = 0;
        }
    }
    return ptr;
}
