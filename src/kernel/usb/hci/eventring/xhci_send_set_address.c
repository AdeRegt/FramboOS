#include "xhci.h"

void xhci_send_set_address(XHCIControllerSession *session, USBDevice* device)
{
    // Maak een Set Address Command TRB aan
    SetAddressCommandTRB* trb = (SetAddressCommandTRB*) xhci_alloc_command_trb(session);
    
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->BSR = 0; // Block Set Address Response
    trb->TRBType = XHCI_TRB_SET_ADDRESS_COMMAND_TRB_TYPE; // Set Address Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst

    uint8_t portspeed = PORTSC_Port_Speed (device->physical_port_id);
    uint16_t calculatedportspeed = 0;
	if(portspeed==XHCI_SPEED_SUPER)
	{
		calculatedportspeed = 512;
	}
	else if(portspeed==XHCI_SPEED_HI)
	{
		calculatedportspeed = 64;
	}
	else if(portspeed==XHCI_SPEED_LOW)
	{
		calculatedportspeed = 8;
	}
	else if(portspeed==XHCI_SPEED_FULL)
	{
		calculatedportspeed = 64;
	}

    void *localring = alloc_page();
	XHCIInputContextBuffer *infostructures = (XHCIInputContextBuffer*) alloc_page();
	infostructures->icc.Aregisters = 0b11;
	infostructures->slotcontext.RootHubPortNumber = device->physical_port_id + 1;
	infostructures->slotcontext.ContextEntries = 1;
	infostructures->slotcontext.Speed = portspeed;
	infostructures->epc.LSA = 0;
	infostructures->epc.EPType = 4;
	infostructures->epc.Cerr = 3;
	infostructures->epc.MaxPacketSize = calculatedportspeed;
	infostructures->epc.TRDequeuePointerLow = ((uint32_t) (uint64_t) localring)>>4 ;
	infostructures->epc.TRDequeuePointerHigh = 0;
	infostructures->epc.DequeueCycleState = 1;
    
	session->device_context_base_address_array[device->slot_id] = ((uint64_t)(infostructures+64));
	
	trb->DataBufferPointerLo = (uint32_t)(uint64_t)(infostructures);
	trb->DataBufferPointerHi = (uint32_t)0;

	device->infostructures = infostructures;
	device->control_endpoint_ring = localring;
	device->control_endpoint_ring_index = 0;
	
    xhci_thingdong(session, device, (void*)trb);

}