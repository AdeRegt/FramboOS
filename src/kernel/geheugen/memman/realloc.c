#include "geheugen.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    size_t size;
    int free;
} AllocationHeader;

#define HEADER_SIZE sizeof(AllocationHeader)

extern void* malloc(size_t size);
extern void free(void* ptr);

void* realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);  /* realloc(NULL, size) == malloc(size) */
    }
    
    if (size == 0) {
        free(ptr);
        return NULL;  /* realloc(ptr, 0) == free(ptr) */
    }
    
    /* Get the old header */
    AllocationHeader *old_hdr = (AllocationHeader*)ptr - 1;
    size_t old_size = old_hdr->size;
    
    /* If new size is smaller or equal, just resize in place */
    if (size <= old_size) {
        old_hdr->size = size;
        return ptr;
    }
    
    /* Allocate new block */
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }
    
    /* Copy old data to new block */
    memcpy(new_ptr, ptr, old_size);
    
    /* Free old block */
    free(ptr);
    
    return new_ptr;
}