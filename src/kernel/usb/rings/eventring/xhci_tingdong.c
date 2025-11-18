#include "xhci.h"

void xhci_thingdong(XHCIControllerSession *session, USBDevice* device, void* trb, int doorbell_index, int doorbell_value)
{
    // Hier komt de thingdong code
    device->pointer_to_requested_trb = (void*)trb;
    DOORBELL[doorbell_index] = doorbell_value;
}