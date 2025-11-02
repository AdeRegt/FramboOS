#include "geheugen.h"

__attribute__((interrupt)) void error_interrupt_handler(interrupt_frame* frame){
  printk("Error interrupt fired!\n");
	asm volatile("cli\nhlt");
}