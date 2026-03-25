#include "xhci.h"

void event_watcher()
{
    //
    // Hier komt de event watcher code
    //
    again:
        //
        // Controleer de Event Ring op nieuwe events
        //

        sleep(1000);
        xhci_check_event();

#ifndef XHCI_XHCI_TREAD
    if(xhci_keep_running)
    {
        goto again;
    }
#else 
    goto again;
#endif
    printk("XHCI: system left loop\n");
}