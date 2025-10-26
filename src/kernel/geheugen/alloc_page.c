#include "geheugen.h"

void* alloc_page(){
    // Eenvoudige pagina allocatie functie
    paging_geheugen_blok->NumberOfPages -= 1;
    if(paging_geheugen_blok->NumberOfPages == 0){
        printk("Waarschuwing: Geen vrije pagina's meer beschikbaar in het paging geheugen blok!\n");
    }
    uint64_t allocated_address = paging_geheugen_blok->VirtualStart;
    paging_geheugen_blok->VirtualStart += MEMORY_PAGE_SIZE;
    memset((void*)allocated_address, 0, MEMORY_PAGE_SIZE);
    return (void*)allocated_address;
}