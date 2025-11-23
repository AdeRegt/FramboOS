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

    void* ring_in = alloc_page();
    void* ring_out = alloc_page();

    device->infostructures->icc.Aregisters = 0b1111;//0b1100
	// device->infostructures->slotcontext.RootHubPortNumber = device->physical_port_id + 1;
	device->infostructures->slotcontext.ContextEntries = 5;
	// device->infostructures->slotcontext.Speed = portspeed;
	device->infostructures->epx[0].LSA = 0;
	device->infostructures->epx[0].EPType = XHCI_ENDPOINT_TYPE_BULK_OUT; // Bulk OUT
	device->infostructures->epx[0].Cerr = 3;
	device->infostructures->epx[0].MaxPacketSize = 512;
	device->infostructures->epx[0].TRDequeuePointerLow = ((uint32_t) (uint64_t) ring_out)>>4 ;
	device->infostructures->epx[0].TRDequeuePointerHigh = 0;
	device->infostructures->epx[0].DequeueCycleState = XHCI_CRCS_DEFAULT_CYCLE_STATE;

	device->infostructures->epx[1].LSA = 0;
	device->infostructures->epx[1].EPType = XHCI_ENDPOINT_TYPE_BULK_IN; // Bulk IN
	device->infostructures->epx[1].Cerr = 3;
	device->infostructures->epx[1].MaxPacketSize = 512;
	device->infostructures->epx[1].TRDequeuePointerLow = ((uint32_t) (uint64_t) ring_in)>>4 ;
	device->infostructures->epx[1].TRDequeuePointerHigh = 0;
	device->infostructures->epx[1].DequeueCycleState = XHCI_CRCS_DEFAULT_CYCLE_STATE;
    
    ConfigureEndpointCommandTRB* trb = (ConfigureEndpointCommandTRB*) xhci_alloc_command_trb(session);
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->TRBType = XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE; // CONGIGURE ENDPOINT Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst
    trb->DataBufferPointerLo = (uint32_t)(uint64_t)(device->infostructures);
	trb->DataBufferPointerHi = (uint32_t)0;

    USBRing* in_ring = (USBRing*) alloc_page();
	in_ring->ring_trbs = ring_in;
	in_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	in_ring->enqueue_index = 0;
	in_ring->slot_id = device->slot_id;
	in_ring->endpoint_id = 2;
	in_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
	device->ep_ring_in = in_ring;

    USBRing* out_ring = (USBRing*) alloc_page();
	out_ring->ring_trbs = ring_out;
	out_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	out_ring->enqueue_index = 0;
	out_ring->slot_id = device->slot_id;
	out_ring->endpoint_id = 3;
	out_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
	device->ep_ring_out = out_ring;

    xhci_thingdong(session, device, (void*)trb, 0, 0);

}