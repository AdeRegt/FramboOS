#include "xhci.h"

INTERRUPT xhci_interrupt_handler(interrupt_frame *frame)
{
    printk("XHCI Interrupt fired!\n");
    for(int i = 0 ; i < xhci_session_count ; i++)
    {
        XHCIControllerSession* session = &xhci_session[i];
        if(USBSTS_EINT)
        {
            USBSTS |= 0b1000;
        }
        IMAN(0) |= 1;
    }
    acknowledge_interrupt();
    xhci_keep_running = 0;
    event_watcher();
}