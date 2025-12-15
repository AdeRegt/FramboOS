#include "serialport.h"

void laad_com()
{
    outportb(COMPORT + 1, 0x00);    // Disable all interrupts
    outportb(COMPORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outportb(COMPORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outportb(COMPORT + 1, 0x00);    //                  (hi byte)
    outportb(COMPORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outportb(COMPORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outportb(COMPORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outportb(COMPORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outportb(COMPORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if(inportb(COMPORT + 0) != 0xAE) {
        printk("serial: serial is niet aanwezig\n");
        return;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outportb(COMPORT + 4, 0x0F);
    printk("serial: serial is aanwezig\n");
    return;
}