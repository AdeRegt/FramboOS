#include "geheugen.h"

void enable_lapic()
{
    uint64_t apic_base = read_msr(IA32_APIC_BASE_MSR);
    apic_base |= IA32_APIC_BASE_ENABLE;  // Set enable bit
    write_msr(IA32_APIC_BASE_MSR, apic_base);

    LAPIC_REG(LAPIC_SVR) = 0x100 | 32; // Enable LAPIC and set spurious interrupt vector to 0xFF
    // printk("LAPIC ingeschakeld op basisadres %x\n", (uint32_t)(apic_base & 0xFFFFF000));
    
    uint32_t id   = LAPIC_REG(0x20);  // LAPIC ID Register
    uint32_t ver  = LAPIC_REG(0x30);  // LAPIC Version Register
    printk("LAPIC ingeschakeld op basisadres %x LAPIC ID: %x, Version: %x\n", (uint32_t)(apic_base & 0xFFFFF000), id , ver);

    //
    // LAPIC instellen met interrupts
    //

    // stel deivision in
    LAPIC_REG(0x3E0) = 0x3; // 0b011 = divide by 16

    // timer vector en mode instellen
    LAPIC_REG(0x320) = (0x20000) | LAPIC_TIMER_VECTOR ; // periodic mode, vector 0xFF

    // afloop instellen
    LAPIC_REG(0x380) = 0x100000; // initial count
}