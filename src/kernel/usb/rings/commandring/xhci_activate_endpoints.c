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

	xhci_set_context(session,device,(usb_endpoint*)&device->configdesc->endpoint1);
	xhci_set_context(session,device,(usb_endpoint*)&device->configdesc->endpoint2);

    ConfigureEndpointCommandTRB* trb = (ConfigureEndpointCommandTRB*) xhci_alloc_command_trb(session);
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->TRBType = XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE; // CONGIGURE ENDPOINT Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst
    trb->DataBufferPointerLo = (uint32_t)(uint64_t)(device->infostructures);
	trb->DataBufferPointerHi = (uint32_t)0;

    xhci_thingdong(session, device, (void*)trb, 0, 0);

}