#include "geheugen.h"

INTERRUPT error_interrupt_handler(interrupt_frame* frame){
  uint8_t int_num = get_active_int();
  printk("Error interrupt fired! %d \n",int_num);
	asm volatile("cli\nhlt");
}