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
	char* namedportspeed;
	if(portspeed==XHCI_SPEED_SUPER)
	{
		calculatedportspeed = 512;
		namedportspeed = "super";
	}
	else if(portspeed==XHCI_SPEED_HI)
	{
		calculatedportspeed = 64;
		namedportspeed = "high";
		return;
	}
	else if(portspeed==XHCI_SPEED_LOW)
	{
		calculatedportspeed = 8;
		namedportspeed = "low";
		return;
	}
	else if(portspeed==XHCI_SPEED_FULL)
	{
		calculatedportspeed = 64;
		namedportspeed = "full";
		return;
	}
	printk("XHCI setup ring for port %d met portspeed %d en snelheid %s \n",device->physical_port_id,calculatedportspeed,namedportspeed);

    void *localring = kalloc();
	void *infostructures = kalloc();
	define_linear_memory_block((void*)(uintptr_t)(localring),1);
	define_linear_memory_block((void*)(uintptr_t)(infostructures),1);
	XHCIInputControlContext *icc = (XHCIInputControlContext*) infostructures;
	icc->Aregisters = 0b11;

	XHCISlotContext *isc = (XHCISlotContext*) (((uint64_t)infostructures) + (xhci_is_64(session)?0x40:0x20));
	isc->RootHubPortNumber = device->physical_port_id + 1;
	isc->ContextEntries = 1;
	isc->Speed = portspeed;

	XHCIEndpointContext *epc = (XHCIEndpointContext*) (((uint64_t)infostructures) + (xhci_is_64(session)?0x80:0x40));
	epc->LSA = 0;
	epc->EPType = 4;
	epc->MaxPacketSize = calculatedportspeed;
	epc->TRDequeuePointerLow = ((uint32_t) (uint64_t) localring)>>4 ;
	epc->TRDequeuePointerHigh = 0;
	epc->DequeueCycleState = 1;
	epc->MaxESITPayloadLow = 2;
	
	session->device_context_base_address_array[device->slot_id] = ((uint64_t)kalloc());
	define_linear_memory_block((void*)(uintptr_t)(session->device_context_base_address_array[device->slot_id]),1);
	
	trb->DataBufferPointerLo = (uint32_t)(uint64_t)(infostructures);
	trb->DataBufferPointerHi = (uint32_t)0;

	device->infostructures = infostructures;

	USBRing* control_ring = (USBRing*) kalloc();
	control_ring->ring_trbs = localring;
	control_ring->ring_size = XHCI_COMMAND_RING_SIZE;
	control_ring->enqueue_index = 0;
	control_ring->slot_id = device->slot_id;
	control_ring->endpoint_id = 1;
	control_ring->cycle_state = XHCI_CRCS_DEFAULT_CYCLE_STATE;
	device->commandring = control_ring;

    xhci_thingdong(session, device, (void*)trb, 0, 0);

}