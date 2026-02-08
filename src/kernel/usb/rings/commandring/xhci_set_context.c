#include "xhci.h"

uint8_t xhci_set_context(XHCIControllerSession* session, USBDevice* device, usb_endpoint* endpoint,XHCIInputContextBuffer* elliot)
{
	uint8_t ep_addr1 = ( endpoint->bEndpointAddress & 0xF ) * 2;
	ep_addr1 += (endpoint->bEndpointAddress & USB_DIR_IN) ? 1 : 0;

	// printk("endpoint: bDescriptorType:%x bEndpointAddress:%x bLength:%x bmAttributes:%x wMaxPacketSize:%x udef:%x \n",endpoint->bDescriptorType,endpoint->bEndpointAddress,endpoint->bLength,endpoint->bmAttributes,endpoint->wMaxPacketSize,endpoint->udef);
    void* ring = alloc_page();

	uint32_t* cv = (uint32_t*)elliot + (0x10 * (ep_addr1-1));
	cv[0] = 0;
	cv[1] = (endpoint->wMaxPacketSize<<16) | ((endpoint->bEndpointAddress & USB_DIR_IN ? XHCI_ENDPOINT_TYPE_BULK_IN : XHCI_ENDPOINT_TYPE_BULK_OUT)<<3) | 0;
	cv[2] = ((uint32_t) (uint64_t) ring) | XHCI_CRCS_DEFAULT_CYCLE_STATE;
	cv[3] = 0;
	cv[4] = 0x400;
	cv[5] = 0;
	cv[6] = 0;
	cv[7] = 0;
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