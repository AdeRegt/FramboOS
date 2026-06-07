#include "geheugen.h"

// Special handler for Page Fault (interrupt 14) with detailed debugging
INTERRUPT error_interrupt_handler_0x0E_(interrupt_frame* frame) {
    uint64_t cr2;
    
    // Read CR2 (faulting address)
    asm volatile("mov %%cr2, %0" : "=r"(cr2) : : "memory");
    
    printk("\n========== PAGE FAULT (INT 0x0E) ==========\n");
    printk("Faulting Address (CR2): %lx\n", cr2);
    printk("Instruction Pointer: %lx\n", frame->ip);
    printk("Code Segment: %lx\n", frame->cs);
    printk("Stack Pointer: %lx\n", frame->sp);
    printk("Stack Segment: %lx\n", frame->ss);
    printk("Flags Register: %lx\n", frame->flags);
    printk("===========================================\n");
    
    cli();
    hlt();
}

#define createinthandler(y) INTERRUPT error_interrupt_handler_##y##_ (interrupt_frame* frame){printk("ERROR INTERRUPT %d FIRED! @ip:%lx sp:%lx \n" , y , frame->ip, frame->sp);cli();hlt();}
createinthandler(0x00)
createinthandler(0x01)
createinthandler(0x02)
createinthandler(0x03)
createinthandler(0x04)
createinthandler(0x05)
createinthandler(0x06)
createinthandler(0x07)
createinthandler(0x08)
createinthandler(0x09)
createinthandler(0x0A)
createinthandler(0x0B)
createinthandler(0x0C)
createinthandler(0x0D)
// Handler for 0x0E is custom above
createinthandler(0x0F)
createinthandler(0x10)
createinthandler(0x11)
createinthandler(0x12)
createinthandler(0x13)
createinthandler(0x14)
createinthandler(0x15)
createinthandler(0x16)
createinthandler(0x17)
createinthandler(0x18)
createinthandler(0x19)
createinthandler(0x1A)
createinthandler(0x1B)
createinthandler(0x1C)
createinthandler(0x1D)
createinthandler(0x1E)
createinthandler(0x1F)