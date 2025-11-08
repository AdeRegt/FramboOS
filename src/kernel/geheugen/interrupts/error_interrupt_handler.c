#include "geheugen.h"

INTERRUPT error_interrupt_handler(interrupt_frame* frame){
  printk("Error interrupt fired!\n");
	asm volatile("cli\nhlt");
}