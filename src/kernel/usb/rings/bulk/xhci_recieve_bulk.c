#include "xhci.h"

void xhci_recieve_bulk(XHCIControllerSession *session, USBDevice* device, uint64_t data_length, void* data){
    xhci_bulk_transfer(session,device,device->ep_ring_in,data_length,data);
}