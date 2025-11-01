#include "geheugen.h"

void geheugen_initialiseer(MemoryInfo *memory_info)
{
    // Hier komt de code om het geheugen te initialiseren
    printk("Geheugen initialisatie gestart...\n");
    allocatie_geheugen_blok = NULL;
    paging_geheugen_blok = NULL;
    memory_info_pointer = memory_info;
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        desc->VirtualStart = desc->PhysicalStart; // Identity mapping instellen
        if(desc->Type == MEMORY_TYPE_CONVENTIONAL_MEMORY && desc->PhysicalStart >= (uint64_t)_KernelEnd)
        {
            if(allocatie_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_ALLOCATION)
            {
                allocatie_geheugen_blok = desc;
                printk("Allocatie geheugen blok gevonden op %lx met grootte %lx pagina's\n", desc->PhysicalStart, desc->NumberOfPages);
            }
            else if(paging_geheugen_blok == NULL && desc->NumberOfPages >= MEMORY_MIN_SIZE_FOR_PAGING) // minimaal 2MB voor paging
            {
                paging_geheugen_blok = desc;
                printk("Paging geheugen blok gevonden op %lx met grootte %lx pagina's\n", desc->PhysicalStart, desc->NumberOfPages);
            }
        }
    }
    kernel_geheugen_blok = geheugenblok_van_address((uint64_t)geheugen_initialiseer);
    if(allocatie_geheugen_blok == NULL || paging_geheugen_blok == NULL || kernel_geheugen_blok == NULL)
    {
        printk("Niet genoeg geheugen gevonden voor allocatie of paging of kernel!\n");for(;;);
    }
    printk("De kernel begint op %lx en eindigd op %lx en beslaat dus %lx \n",_KernelStart, _KernelEnd, (_KernelEnd - _KernelStart));
    
    #ifdef ENABLE_GDT 
    initialise_gdt();
    printk("GDT geactiveerd.\n");
    #else 
    printk("waarschuwing: GDT is uitgeschakeld.\n");
    #endif 

    #ifdef ENABLE_PAGING
    master_page_table = alloc_page();
    // en nu de precieze mapping volgens de memory descriptors
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        if(desc->Type == MEMORY_TYPE_CONVENTIONAL_MEMORY || desc->Type == MEMORY_TYPE_BOOT_SERVICES_CODE || desc->Type == MEMORY_TYPE_BOOT_SERVICES_DATA || desc->Type == MEMORY_TYPE_ACPI_RECLAIM_MEMORY || desc->Type == MEMORY_TYPE_ACPI_MEMORY_NVS){
            define_page_memory_range_from_memory_descriptor(desc);
        }
    }
    define_page_memory_range_from_memory_descriptor(allocatie_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(paging_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(kernel_geheugen_blok);
    // asm volatile ("mov %0, %%cr3" : : "r" (master_page_table));
    #else 
    printk("waarschuwing: Paging is uitgeschakeld.\n");
    #endif

    #ifdef ENABLE_INTERUPTS
    cli();
    reset_pic();
    memset(&idtr, 0, sizeof(IDTR));
    memset(idt, 0, sizeof(IDTDescEntry) * IDT_MAX_DESCRIPTORS);
    idtr.Offset = (uintptr_t)&idt[0];
    idtr.Limit = (uint16_t)sizeof(IDTDescEntry) * IDT_MAX_DESCRIPTORS - 1;
    idt_set_entry(&idt[0x00], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x01], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x02], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x03], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x04], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x05], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x06], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x07], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x08], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x09], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0A], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0B], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0C], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0D], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0E], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0F], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x10], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x11], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x12], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x13], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x14], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x15], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x16], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x17], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x18], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x19], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1A], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1B], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1C], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1D], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1E], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1F], error_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x20], default_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x21], default_interrupt_handler, 0x08, 0, IDT_TYPE_INTERRUPT_GATE);
    asm volatile ("lidt %0" : : "m"(idtr));
    sti();
    printk("PIC reset\n");
    #else 
    printk("waarschuwing: Interrupts zijn uitgeschakeld.\n");
    #endif
}