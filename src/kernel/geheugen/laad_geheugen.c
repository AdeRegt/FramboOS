#include "geheugen.h"

void laad_geheugen(BootInfo *meme)
{
    MemoryInfo *memory_info = meme->memory_info;
    // Hier komt de code om het geheugen te initialiseren
    printk("Geheugen initialisatie gestart...\n");
    allocatie_geheugen_blok = NULL;
    paging_geheugen_blok = NULL;
    memory_info_pointer = memory_info;
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        if(desc->VirtualStart!=desc->PhysicalStart&&desc->VirtualStart){
            printk("Fout: Niet-identity mapping gevonden in memory map! PhysicalStart: %lx, VirtualStart: %lx\n", desc->PhysicalStart, desc->VirtualStart);
            for(;;);
        }
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
    kernel_geheugen_blok = geheugenblok_van_address((uint64_t)laad_geheugen);
    video_geheugen_blok = geheugenblok_van_address((uint64_t)meme->graphics_info->BaseAddress);
    if(allocatie_geheugen_blok == NULL)
    {
        printk("Niet genoeg geheugen gevonden voor allocatie\n");for(;;);
    }
    if(paging_geheugen_blok == NULL)
    {
        printk("Niet genoeg geheugen gevonden voor paging\n");for(;;);
    }
    if(kernel_geheugen_blok == NULL)
    {
        printk("Niet genoeg geheugen gevonden voor kernel\n");for(;;);
    }
    printk("De kernel begint op %lx en eindigd op %lx en beslaat dus %lx \n",_KernelStart, _KernelEnd, (_KernelEnd - _KernelStart));
    
    #ifdef ENABLE_GDT 
    initialise_gdt();
    #else 
    printk("waarschuwing: GDT is uitgeschakeld.\n");
    #endif 

    #ifdef ENABLE_PAGING
    master_page_table = alloc_page();
    //en nu de precieze mapping volgens de memory descriptors
    for(uint64_t offset = 0; offset < memory_info->mMapSize; offset += memory_info->mMapDescSize)
    {
        MemoryDescriptor *desc = (MemoryDescriptor *)((uint64_t)memory_info->mMap + offset);
        if(desc->Type == MEMORY_TYPE_BOOT_SERVICES_CODE || desc->Type == MEMORY_TYPE_BOOT_SERVICES_DATA || desc->Type == MEMORY_TYPE_ACPI_RECLAIM_MEMORY || desc->Type == MEMORY_TYPE_ACPI_MEMORY_NVS){
            define_page_memory_range_from_memory_descriptor(desc);
        }
    }
    for(uint64_t valve = 0 ; valve < (0xFFFFF000/PAGE_GAP_SIZE) ; valve++){
        define_linear_memory_block((void*)(valve*PAGE_GAP_SIZE));
    }
    define_page_memory_range_from_memory_descriptor(allocatie_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(paging_geheugen_blok);
    define_page_memory_range_from_memory_descriptor(kernel_geheugen_blok);
    map_memory(master_page_table, (void*)meme->graphics_info->BaseAddress, (void*)meme->graphics_info->BaseAddress);
    for(int i = 0 ; i < 5 ; i++){
        map_memory(master_page_table, (void*)((uint64_t)meme->graphics_info->BaseAddress + (i*PAGE_GAP_SIZE)), (void*)((uint64_t)meme->graphics_info->BaseAddress + (i*PAGE_GAP_SIZE)));
    }
    asm volatile ("mov %0, %%cr3" : : "r" (master_page_table));
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
    idt_set_entry(&idt[0x00], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x01], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x02], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x03], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x04], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x05], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x06], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x07], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x08], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x09], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0A], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0B], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0C], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0D], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0E], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x0F], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x10], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x11], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x12], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x13], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x14], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x15], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x16], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x17], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x18], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x19], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1A], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1B], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1C], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1D], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1E], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    idt_set_entry(&idt[0x1F], error_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    for(int i = IDT_OFFSET ; i < 0xFF ; i++)
    {
        idt_set_entry(&idt[i], default_interrupt_handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    }
    current_task = 0;
    max_task = 1;
    memset(&ct, 0, sizeof(task_t)*50);
    memcpy(ct[0].name, "Kernel main loop", 32);
    idt_set_entry(&idt[LAPIC_TIMER_VECTOR], taskswitchstub, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    asm volatile ("lidt %0" : : "m"(idtr));
    sti();
    #else 
    printk("waarschuwing: Interrupts zijn uitgeschakeld.\n");
    #endif
}