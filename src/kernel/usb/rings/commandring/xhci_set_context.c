#include "xhci.h"

USBRing* xhci_set_context(XHCIEndpointContext* context, uint8_t ep_type, uint16_t packetsize)
{
    void* ring = alloc_page();

    context->LSA = 0;
	context->EPType = ep_type;
	context->Cerr = 3;
	context->MaxPacketSize = packetsize;
	context->TRDequeuePointerLow = ((uint32_t) (uint64_t) ring)>>4 ;
	context->TRDequeuePointerHigh = 0;
	context->DequeueCycleState = XHCI_CRCS_DEFAULT_CYCLE_STATE;

	USBRing* control_ring = (USBRing*) alloc_page();
	control_ring->ring_trbs = ring;
	control_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	control_ring->enqueue_index = 0;
	control_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
    return control_ring;
}