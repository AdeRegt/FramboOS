#include "geheugen.h"

__attribute__((interrupt)) void default_interrupt_handler(interrupt_frame* frame){
  printk("Interrupt fired!\n");
  outportb(0xA0,0x20);
	outportb(0x20, 0x20); // EOI naar PIC sturen
}