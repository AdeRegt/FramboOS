#include "geheugen.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    int free;
} AllocationHeader;

#define HEADER_SIZE sizeof(AllocationHeader)

void free(void* ptr) {
    if (ptr == NULL) {
        return;  /* free(NULL) is safe */
    }
    
    /* Get the header just before the pointer */
    AllocationHeader *hdr = (AllocationHeader*)ptr - 1;
    
    /* Mark as free */
    hdr->free = 1;
}
