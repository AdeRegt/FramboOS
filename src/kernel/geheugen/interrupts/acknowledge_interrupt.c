#include "geheugen.h"

void acknowledge_interrupt() {
    LAPIC_REG(0xB0) = 0; // Stuur EOI naar LAPIC
    // outportb(0xA0, 0x20); // Stuur EOI naar slave PIC
    // outportb(0x20, 0x20); // Stuur EOI naar master PIC
}