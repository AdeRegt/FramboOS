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

        sleep(100);
        
        xhci_check_event();
    goto again;
}