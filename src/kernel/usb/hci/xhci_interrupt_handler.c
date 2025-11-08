#include "xhci.h"

INTERRUPT xhci_interrupt_handler(interrupt_frame *frame)
{
    printk("XHCI Interrupt fired!\n");for(;;);
    acknowledge_interrupt();
}