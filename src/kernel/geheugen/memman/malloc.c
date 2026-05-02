#include "geheugen.h"
#include <stddef.h>
#include <stdint.h>

/* Simple malloc/free allocator for kernel Lua */

typedef struct {
    size_t size;
    int free;
} AllocationHeader;

#define HEADER_SIZE sizeof(AllocationHeader)

static uint64_t region_start = (uint64_t)-1;   /* Start of memory region (sentinel: -1 = not initialized) */
static uint64_t region_end = 0;                /* End of memory region */
static uint64_t watermark = 0;                 /* High water mark of allocated memory */

/* Initialize allocator at first call */
static void init_allocator(void) {
    if (region_start == (uint64_t)-1 && allocatie_geheugen_blok != NULL) {
        region_start = allocatie_geheugen_blok->PhysicalStart;
        region_end = allocatie_geheugen_blok->PhysicalStart + 
                     (allocatie_geheugen_blok->NumberOfPages * MEMORY_PAGE_SIZE);
        watermark = region_start;
    }
}

void* malloc(size_t size) {
    if (size == 0) return NULL;
    
    init_allocator();
    
    if (region_start == (uint64_t)-1) {
        printk("MALLOC ERROR: allocator not initialized\n");
        return NULL;  /* Allocator not initialized */
    }
    
    printk("malloc(%x) called\n", (uint64_t)size);
    
    /* First-fit: search for previously freed blocks */
    uint64_t current = region_start;
    int iterations = 0;
    const int MAX_ITERATIONS = 100000;  /* Safety limit */
    
    while (current < watermark) {
        if (iterations++ > MAX_ITERATIONS) {
            printk("MALLOC ERROR: loop exceeded iterations\n");
            return NULL;
        }
        
        AllocationHeader *hdr = (AllocationHeader*)current;
        
        /* Sanity check on header */
        if (hdr->size == 0 || hdr->size > (region_end - current - HEADER_SIZE)) {
            printk("MALLOC ERROR: corrupt header at %x\n", current);
            return NULL;
        }
        
        /* Check if block is marked as free and large enough */
        if (hdr->free && hdr->size >= size) {
            /* Reuse this block */
            hdr->free = 0;
            
            /* If block is much larger than needed, split it */
            if (hdr->size > size + HEADER_SIZE) {
                uint64_t remainder_addr = current + HEADER_SIZE + size;
                AllocationHeader *remainder_hdr = (AllocationHeader*)remainder_addr;
                remainder_hdr->size = hdr->size - size - HEADER_SIZE;
                remainder_hdr->free = 1;
                hdr->size = size;
            }
            
            uint64_t result = current + HEADER_SIZE;
            printk("malloc(%x) -> %x (reused)\n", (uint64_t)size, result);
            return (void*)result;
        }
        
        current += HEADER_SIZE + hdr->size;
    }
    
    /* Allocate new memory at watermark */
    if (watermark + HEADER_SIZE + size > region_end) {
        printk("MALLOC ERROR: out of memory\n");
        return NULL;  /* Out of memory */
    }
    
    AllocationHeader *hdr = (AllocationHeader*)watermark;
    hdr->size = size;
    hdr->free = 0;
    
    uint64_t result = watermark + HEADER_SIZE;
    watermark += HEADER_SIZE + size;
    
    printk("malloc(%x) -> %x (new)\n", (uint64_t)size, result);
    return (void*)result;
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
