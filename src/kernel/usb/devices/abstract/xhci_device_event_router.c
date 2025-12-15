#include "xhci.h"

void xhci_device_event_router(XHCIControllerSession *session, USBDevice* device)
{
    USBRing* rin = device->ep_ring_in;
    void* thingi = alloc_page();
    xhci_send_bulk(session,device,rin,0,thingi);
    // printk("--> cs=%x ep=%x ei=%x sid=%x ron=%x AA\n",rin->cycle_state,rin->endpoint_id,rin->enqueue_index,rin->slot_id,rin->ring_trbs);
}