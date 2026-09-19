#include "xhci.h"

void event_watcher(int timeout)
{
    //
    // Hier komt de event watcher code
    //
    again:
        //
        // Controleer de Event Ring op nieuwe events
        //
        sleep(50);
        int ur = xhci_check_event();

#ifndef XHCI_XHCI_TREAD
    if(xhci_keep_running)
    {
        if(timeout)
        {
            if(!ur)
            {
                timeout--;
            }
            goto again;
        }
        else
        {
        }
    }
#else 
    goto again;
#endif
    // printk("XHCI: system left loop\n");
}