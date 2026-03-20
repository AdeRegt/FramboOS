#include "xhci.h"

void xhci_send_bulk(XHCIControllerSession *session, USBDevice* device, uint64_t data_length, void* data){
    xhci_bulk_transfer(session,device,device->ep_ring_out,data_length,data);
}