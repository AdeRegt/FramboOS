#include "xhci.h"

void xhci_ring_trb_ring(XHCIControllerSession *session, USBDevice* device, USBRing *ring, void* trb)
{
    xhci_thingdong(session, device, trb, ring->slot_id, ring->endpoint_id);
}
