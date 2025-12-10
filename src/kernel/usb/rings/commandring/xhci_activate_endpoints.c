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

    XHCIInputContextBuffer* elliot = alloc_page();
    uint8_t ib = 0;
    if(device->epfostructures1==NULL){
        device->epfostructures1 = elliot;
        ib = xhci_set_context(session,device,(usb_endpoint*)&device->configdesc->endpoint1,elliot);
    }else if(device->epfostructures2==NULL){
        device->epfostructures2 = elliot;
        ib = xhci_set_context(session,device,(usb_endpoint*)&device->configdesc->endpoint2,elliot);
    }else{
        printk("crf\n");for(;;);
    }
	

	elliot->icc.Aregisters = 1 | (1<<ib);//(1<<ia) | (1<<ib);
	elliot->slotcontext.ContextEntries = ib;//ia>ib?ia:ib;
    elliot->slotcontext.RootHubPortNumber = device->physical_port_id + 1;
	// device->epfostructures->slotcontext.ContextEntries = 1;
	elliot->slotcontext.Speed = 4;

    ConfigureEndpointCommandTRB* trb = (ConfigureEndpointCommandTRB*) xhci_alloc_command_trb(session);
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->TRBType = XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE; // CONGIGURE ENDPOINT Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst
    trb->DataBufferPointerLo = (uint32_t)(uint64_t)(elliot);
	trb->DataBufferPointerHi = (uint32_t)0;

    // uint32_t* vxc = (uint32_t*) device->epfostructures;
    // for(int i = 0 ; i < 100 ; i++){
    //     if(vxc[i]){
    //         printk("[%x:%x] ",i,vxc[i]);
    //     }
    // }
    // for(;;);

    xhci_thingdong(session, device, (void*)trb, 0, 0);

}