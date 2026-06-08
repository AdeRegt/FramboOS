#include "geheugen.h"

void map_memory(void* pml4mem, void *virtualmemory, void* physicalmemory, uint8_t is_user, uint8_t is_cache) {
    // Definieer de maskers voor 2MB uitlijning (de onderste 21 bits moeten 0 worden)
    uint64_t vaddr = (uint64_t)virtualmemory & ~0x1FFFFF;
    uint64_t paddr = (uint64_t)physicalmemory & ~0x1FFFFF;

    // Nu gebruiken we de netjes uitgelijnde vaddr voor de indexer
    PageLookupResult lookup = page_map_indexer(vaddr);
    PageTable* PML4 = (PageTable*) pml4mem;

    // 1. PML4 -> PDP
    Page* pml4_entry = &PML4->pages[lookup.page_map_level_4_table_index];
    PageTable *PDP;
    if (!pml4_entry->present) {
        PDP = (PageTable*) alloc_page();
        memset(PDP, 0, 0x1000);
        pml4_entry->address = (uint64_t)PDP >> 12;
        pml4_entry->present = 1;
        pml4_entry->readwrite = 1;
        pml4_entry->usersuper = 1; 
    } else {
        PDP = (PageTable*)((uint64_t)pml4_entry->address << 12);
    }

    // 2. PDP -> PD
    Page* pdp_entry = &PDP->pages[lookup.page_directory_pointer_table_index];
    PageTable *PD;
    if (!pdp_entry->present) {
        PD = (PageTable*) alloc_page();
        memset(PD, 0, 0x1000);
        pdp_entry->address = (uint64_t)PD >> 12;
        pdp_entry->present = 1;
        pdp_entry->readwrite = 1;
        pdp_entry->usersuper = 1;
    } else {
        PD = (PageTable*)((uint64_t)pdp_entry->address << 12);
    }

    // 3. PD -> Fysieke 2MB Pagina
    Page* pd_entry = &PD->pages[lookup.page_directory_table_index];
    
    // We gebruiken hier het netjes uitgelijnde fysieke adres (paddr)
    pd_entry->address           = paddr >> 12;
    pd_entry->present           = 1;
    pd_entry->readwrite         = 1;
    pd_entry->largepages        = 1; // Vertel de CPU dat dit een 2MB pagina is
    pd_entry->usersuper         = is_user;
    pd_entry->cachedisabled     = is_cache; // Bit 4 voorkomt dat de CPU registers cachet
}