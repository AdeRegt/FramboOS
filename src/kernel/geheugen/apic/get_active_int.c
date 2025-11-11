#include "geheugen.h"

uint8_t get_active_int()
{
    uint32_t apic_base = read_msr(IA32_APIC_BASE_MSR) & 0xFFFFF000;
    uint32_t irr_offset = apic_base + 0x200; // IRR starts at offset 0x200
    for(int i = 3; i >= 0; i--) {
        uint32_t irr_reg = LAPIC_REG(0x200+(i * 0x10));
        if(irr_reg != 0) {
            for(int bit = 31; bit >= 0; bit--) {
                if(irr_reg & (1 << bit)) {
                    return (i * 32) + bit;
                }
            }
        }
    }
    return 0xFF; // Geen actieve interrupt gevonden
}