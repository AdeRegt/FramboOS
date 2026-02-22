#include "xhci.h"

uint8_t xhci_set_context(XHCIControllerSession* session, USBDevice* device, usb_endpoint* endpoint,void* elliot)
{
	uint8_t ep_addr1 = ( endpoint->bEndpointAddress & 0xF ) * 2;
	ep_addr1 += (endpoint->bEndpointAddress & USB_DIR_IN) ? 1 : 0;

	// printk("endpoint: bDescriptorType:%x bEndpointAddress:%x bLength:%x bmAttributes:%x wMaxPacketSize:%x udef:%x \n",endpoint->bDescriptorType,endpoint->bEndpointAddress,endpoint->bLength,endpoint->bmAttributes,endpoint->wMaxPacketSize,endpoint->udef);
    void* ring = alloc_page();

	// uint32_t switcher = xhci_is_64(session)?0x40:0x20;
	// uint32_t* cv = (uint32_t*)elliot + (switcher * (ep_addr1-1));

	uint32_t context_size = xhci_is_64(session) ? 64 : 32;
	uint32_t context_dw   = context_size / 4;

	uint8_t ep_num = endpoint->bEndpointAddress & 0xF;
	uint8_t dir    = (endpoint->bEndpointAddress & USB_DIR_IN) ? 1 : 0;

	/* Compute DCI */
	uint8_t dci = ep_num * 2 + dir;

	/* Compute pointer */
	uint32_t* base = (uint32_t*) (uint64_t) (void*)elliot;
	uint32_t* ep_ctx = base + (dci * context_dw);
	ep_ctx[0] = 0;
	ep_ctx[1] = (endpoint->wMaxPacketSize<<16) | ((endpoint->bEndpointAddress & USB_DIR_IN ? XHCI_ENDPOINT_TYPE_BULK_IN : XHCI_ENDPOINT_TYPE_BULK_OUT)<<3) | 0;
	ep_ctx[2] = ((uint32_t) (uint64_t) ring) | XHCI_CRCS_DEFAULT_CYCLE_STATE;
	ep_ctx[3] = 0;
	ep_ctx[4] = 0x400;
	ep_ctx[5] = 0;
	ep_ctx[6] = 0;
	ep_ctx[7] = 0;
	// printk("@ %d %x %x %x %x %x %x %x %x \n",ep_addr1, cv[0], cv[1], cv[2], cv[3], cv[4], cv[5], cv[6], cv[7]);

	USBRing* control_ring = (USBRing*) alloc_page();
	control_ring->ring_trbs = ring;
	control_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	control_ring->enqueue_index = 0;
	control_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
	control_ring->endpoint_id = ep_addr1;
	control_ring->slot_id = device->slot_id;
    
	if(endpoint->bEndpointAddress & USB_DIR_IN){
		printk("stdin\n");
		device->ep_ring_in = control_ring;
	}else{
		printk("stdout\n");
		device->ep_ring_out = control_ring;
	}

	return ep_addr1;
}