#include "xhci.h"

void xhci_thingdong(XHCIControllerSession *session, USBDevice* device, void* trb)
{
    // Hier komt de thingdong code
    device->pointer_to_requested_trb = (void*)trb;
    DOORBELL[0] = 0;
}