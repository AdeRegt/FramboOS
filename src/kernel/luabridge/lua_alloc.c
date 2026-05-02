#include "lua_kernel.h"
#include <stddef.h>
#include "debugger.h"

extern void* malloc(size_t size);
extern void* realloc(void* ptr, size_t size);
extern void free(void* ptr);

/* Lua memory allocator - use kernel malloc/free */
void* lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    (void)ud;
    (void)osize;
    
    if (nsize == 0) {
        /* Free memory */
        if (ptr != NULL) {
            free(ptr);
        }
        return NULL;
    } else {
        /* Allocate/reallocate memory */
        if (ptr == NULL) {
            return malloc(nsize);
        } else {
            return realloc(ptr, nsize);
        }
    }
}