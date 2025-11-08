#include "xhci.h"

void wait_for_controller_not_ready()
{
    //
    // Wacht tot de controller niet meer klaar is
    //
    while (USBSTS & USBSTS_CNR)
    {
        // Wachten
        sleep(10);
    }
}