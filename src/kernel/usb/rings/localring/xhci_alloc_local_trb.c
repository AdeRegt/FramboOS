#include "xhci.h"

void* xhci_alloc_local_trb(XHCIControllerSession *session, USBDevice* device)
{
    void* msg = &((uint32_t*)device->control_endpoint_ring)[device->control_endpoint_ring_index];
    memset(msg, 0, sizeof(SetAddressCommandTRB)); // Clear the TRB memory
    device->control_endpoint_ring_index += 4;
    return msg;
}