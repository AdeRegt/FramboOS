#include "geheugen.h"

void map_memory(void* pml4mem, void *virtualmemory,void* physicalmemory){
    PageLookupResult lookup = page_map_indexer((uint64_t)virtualmemory);
    PageTable* PLM4 = (PageTable*) pml4mem;
    Page PDE = PLM4->pages[lookup.page_map_level_4_table_index];
    PageTable *PDP;
    if(!PDE.present){
        PDP = (PageTable*) alloc_page();
        memset(PDP, 0, 0x1000);
        PDE.address = (uint64_t)PDP >> 12;
        PDE.present = 1;
        PDE.readwrite = 1;
        // PDE.usersuper   = 1;
        PLM4->pages[lookup.page_map_level_4_table_index] = PDE;
    }else{
        PDP = (PageTable*)((uint64_t)PDE.address<<12);
    }

    PDE = PDP->pages[lookup.page_directory_pointer_table_index];
    PageTable *PD;
    if(!PDE.present){
        PD = (PageTable*) alloc_page();
        memset(PD, 0, 0x1000);
        PDE.address = (uint64_t)PD >> 12;
        PDE.present = 1;
        PDE.readwrite = 1;
        // PDE.usersuper   = 1;
        PDP->pages[lookup.page_directory_pointer_table_index] = PDE;
    }else{
        PD = (PageTable*)((uint64_t)PDE.address<<12);
    }

    PDE = PD->pages[lookup.page_directory_table_index];
    PageTable *PE;
    PDE.address     = (uint64_t)physicalmemory>>12;//PE >> 12;
    PDE.present     = 1;
    PDE.readwrite   = 1;
    PDE.largepages  = 1;
    // PDE.usersuper   = 1;
    PD->pages[lookup.page_directory_table_index] = PDE;
}