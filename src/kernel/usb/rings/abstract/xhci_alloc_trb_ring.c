#include "xhci.h"

void *xhci_alloc_trb_ring(USBRing *ring)
{
    void* msg = &((uint32_t*)ring->ring_trbs)[ring->enqueue_index];
    memset(msg, 0, sizeof(SetAddressCommandTRB)); // Clear the TRB memory
    ring->enqueue_index += 4;
    if(ring->ring_size-1==(ring->enqueue_index/4)){
        printk("EEEEEEE\n");
    }
    return msg;
}