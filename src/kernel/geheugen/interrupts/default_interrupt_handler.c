#include "geheugen.h"

INTERRUPT default_interrupt_handler(interrupt_frame* frame){
  uint8_t int_num = get_active_int();
  printk("Onbekende interrupt ontvangen: %d\n", int_num);
  acknowledge_interrupt();
}