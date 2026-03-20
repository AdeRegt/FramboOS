#include "xhci.h"

void xhci_activate_endpoints(XHCIControllerSession *session, USBDevice* device)
{
    if(device->configdesc==NULL)
    {
        //
        // we just focus on the MSD
        return;
    }

    if(device->configdesc->interfacdesc.bNumEndpoints != 2)
    {
        printk("XHCI: het apparaat heeft geen 2 endpoints, activatie overgeslagen!\n");
        return;
    }

    void* mostar = alloc_page();
    void* ring1 = alloc_page();
    void* ring2 = alloc_page();

    uint8_t ep_addr1 = ( device->configdesc->endpoint1.bEndpointAddress & 0xF ) * 2;
	ep_addr1 += (device->configdesc->endpoint1.bEndpointAddress & USB_DIR_IN) ? 1 : 0;

    uint8_t ep_addr2 = ( device->configdesc->endpoint2.bEndpointAddress & 0xF ) * 2;
	ep_addr2 += (device->configdesc->endpoint2.bEndpointAddress & USB_DIR_IN) ? 1 : 0;

    XHCIInputControlContext *icc = (XHCIInputControlContext*) mostar;
	icc->Aregisters = (1<<ep_addr1)|(1<<ep_addr2)|1;
    icc->Dregisters = 0;

    uint8_t portspeed = PORTSC_Port_Speed (device->physical_port_id);

    XHCISlotContext *isc = (XHCISlotContext*) (((uint64_t)mostar) + (xhci_is_64(session)?0x40:0x20));
	isc->RootHubPortNumber = device->physical_port_id + 1;
	isc->ContextEntries = 4;
	isc->Speed = portspeed;

    uint32_t windowA = ((ep_addr1+1)*(xhci_is_64(session)?0x40:0x20));
    uint32_t windowB = ((ep_addr2+1)*(xhci_is_64(session)?0x40:0x20));

	XHCIEndpointContext *epc1 = (XHCIEndpointContext*) (((uint64_t)mostar) + windowA);
	epc1->MaxPacketSize = device->configdesc->endpoint1.wMaxPacketSize;
    epc1->EPType = device->configdesc->endpoint1.bEndpointAddress & USB_DIR_IN ? XHCI_ENDPOINT_TYPE_BULK_IN : XHCI_ENDPOINT_TYPE_BULK_OUT;
    epc1->DequeueCycleState = 1;
    epc1->TRDequeuePointerLow = ((uint32_t) (uint64_t) ring1)>>4;
    epc1->Cerr = 3;
    epc1->AverageTRBLength = device->configdesc->endpoint1.wMaxPacketSize;
    epc1->MaxBurstSize = 3;
    
	XHCIEndpointContext *epc2 = (XHCIEndpointContext*) (((uint64_t)mostar) + windowB);
    epc2->MaxPacketSize = device->configdesc->endpoint2.wMaxPacketSize;
    epc2->EPType = device->configdesc->endpoint2.bEndpointAddress & USB_DIR_IN ? XHCI_ENDPOINT_TYPE_BULK_IN : XHCI_ENDPOINT_TYPE_BULK_OUT;
    epc2->DequeueCycleState = 1;
    epc2->TRDequeuePointerLow = ((uint32_t) (uint64_t) ring2)>>4;
    epc2->Cerr = 3;
    epc2->AverageTRBLength = device->configdesc->endpoint2.wMaxPacketSize;
    epc2->MaxBurstSize = 3;
    
    ConfigureEndpointCommandTRB* trb = (ConfigureEndpointCommandTRB*) xhci_alloc_command_trb(session);
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->TRBType = XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE; // CONGIGURE ENDPOINT Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst
    trb->DataBufferPointerLo = (uint32_t)(uint64_t)(mostar);
	trb->DataBufferPointerHi = (uint32_t)0;

    USBRing* ringdec1 = (USBRing*) alloc_page();
    ringdec1->cycle_state = 1;
    ringdec1->endpoint_id = ep_addr1;
    ringdec1->ring_size = 20;
    ringdec1->slot_id = device->slot_id;
    ringdec1->ring_trbs = ring1;

    USBRing* ringdec2 = (USBRing*) alloc_page();
    ringdec2->cycle_state = 1;
    ringdec2->endpoint_id = ep_addr2;
    ringdec2->ring_size = 20;
    ringdec2->slot_id = device->slot_id;
    ringdec2->ring_trbs = ring2;

    if(device->configdesc->endpoint1.bEndpointAddress & USB_DIR_IN ){
        device->ep_ring_in = ringdec1;
        device->ep_ring_out = ringdec2;
    }else{
        device->ep_ring_in = ringdec2;
        device->ep_ring_out = ringdec1;
    }
    
    xhci_thingdong(session, device, (void*)trb, 0, 0);

}