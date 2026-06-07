#include "geheugen.h"

void* kalloc(){
    void* thing = alloc_page();
    define_linear_memory_block(thing);
    return thing;
}