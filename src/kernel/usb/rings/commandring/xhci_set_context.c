#include "xhci.h"

void xhci_set_context(XHCIControllerSession* session, USBDevice* device, usb_endpoint* endpoint)
{
	unsigned char ep_addr1 = ( endpoint->bEndpointAddress & 0xF ) * 2;
	ep_addr1 += (endpoint->bEndpointAddress & USB_DIR_IN) ? 1 : 0;

    void* ring = alloc_page();

	XHCIEndpointContext context = ((XHCIEndpointContext*)&device->infostructures->ep0)[ep_addr1];

    context.LSA = 0;
	context.EPType = endpoint->bEndpointAddress & USB_DIR_IN ? XHCI_ENDPOINT_TYPE_BULK_IN : XHCI_ENDPOINT_TYPE_BULK_OUT;
	context.Cerr = 3;
	context.MaxPacketSize = endpoint->wMaxPacketSize;
	context.TRDequeuePointerLow = ((uint32_t) (uint64_t) ring)>>4 ;
	context.TRDequeuePointerHigh = 0;
	context.DequeueCycleState = XHCI_CRCS_DEFAULT_CYCLE_STATE;

	USBRing* control_ring = (USBRing*) alloc_page();
	control_ring->ring_trbs = ring;
	control_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	control_ring->enqueue_index = 0;
	control_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
    
	if(endpoint->bEndpointAddress & USB_DIR_IN){
		device->ep_ring_in = control_ring;
	}else{
		device->ep_ring_out = control_ring;
	}
}