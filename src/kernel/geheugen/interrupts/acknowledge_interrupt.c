#include "geheugen.h"

void acknowledge_interrupt() {
    outportb(0xA0, 0x20); // Stuur EOI naar slave PIC
    outportb(0x20, 0x20); // Stuur EOI naar master PIC
}