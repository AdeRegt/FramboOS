#include "geheugen.h"

INTERRUPT default_interrupt_handler(interrupt_frame* frame){
  printk("Interrupt fired!\n");
  acknowledge_interrupt();
}