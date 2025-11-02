#include "geheugen.h"

void reset_pic(){
    // Herstel de PIC naar de standaard IRQ-mapping
    outportb(0x20, 0x11); // Initialiseer master PIC
    outportb(0xA0, 0x11); // Initialiseer slave PIC
    outportb(0x21, 0x20); // Stel vector offset voor master PIC in op 0x20
    outportb(0xA1, 0x28); // Stel vector offset voor slave PIC in op 0x28
    outportb(0x21, 0x04); // Configureer master PIC om slave PIC te gebruiken op IRQ2
    outportb(0xA1, 0x02); // Configureer slave PIC als verbonden met IRQ2 van master PIC
    outportb(0x21, 0x01); // Stel modus van master PIC in op 8086/88 (MCS-80/85) modus
    outportb(0xA1, 0x01); // Stel modus van slave PIC in op 8086/88 (MCS-80/85) modus
    outportb(0x21, 0x0);  // Schakel alle IRQ's op master PIC in
    outportb(0xA1, 0x0);  // Schakel alle IRQ's op slave PIC in
}