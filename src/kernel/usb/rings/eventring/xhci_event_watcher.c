#include "xhci.h"
#include "beeldscherm.h"

void event_watcher(int timeout)
{
    sleep(1000);
    beeldscherm_leeg();
    printk("Started the thing\n");

    //
    // Hier komt de event watcher code
    //
    again:
        //
        // Controleer de Event Ring op nieuwe events
        //
        sleep(100);
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