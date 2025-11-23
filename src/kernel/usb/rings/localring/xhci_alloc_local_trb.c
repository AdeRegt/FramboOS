#include "xhci.h"

void* xhci_alloc_local_trb(XHCIControllerSession *session, USBDevice* device)
{
    return xhci_alloc_trb_ring((USBRing*)device->commandring);
}