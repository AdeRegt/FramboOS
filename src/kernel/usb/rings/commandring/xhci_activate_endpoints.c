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

    uint64_t offsetdinges = ((uint64_t)&device->configdesc->configdesc) + device->configdesc->configdesc.bLength + device->configdesc->interfacdesc.bLength;
    usb_endpoint endpoint1 = ((usb_endpoint*)offsetdinges)[0];
    offsetdinges += 13;
    usb_endpoint endpoint2 = ((usb_endpoint*)offsetdinges)[0];
    
    usb_endpoint* in_endpoint = NULL;
    usb_endpoint* out_endpoint = NULL;
    if( (endpoint1.bEndpointAddress & 0x80) != 0 ){
        in_endpoint = &endpoint1;
        out_endpoint = &endpoint2;
    }else{
        in_endpoint = &endpoint2;
        out_endpoint = &endpoint1;
    }

    void* ring_in = alloc_page();
    void* ring_out = alloc_page();

    uint8_t index1 = (( in_endpoint->bEndpointAddress & 0x0F ) * 2)+1;
    uint8_t index2 = (out_endpoint->bEndpointAddress & 0x0F)*2;

    device->infostructures->epx[index1-1].EPType = 3;
    device->infostructures->epx[index1-1].MaxPacketSize = 512*2;
    device->infostructures->epx[index1-1].Cerr = 3;
    device->infostructures->epx[index1-1].MaxESITPayloadLow = ((uint32_t)(uint64_t) ring_in) | 1;

    device->infostructures->epx[index2-1].EPType = 2;
    device->infostructures->epx[index2-1].MaxPacketSize = 512*2;
    device->infostructures->epx[index2-1].Cerr = 3;
    device->infostructures->epx[index2-1].MaxESITPayloadLow = ((uint32_t)(uint64_t) ring_out) | 1;

    device->infostructures->slotcontext.ContextEntries = 4;
    
    ConfigureEndpointCommandTRB* trb = (ConfigureEndpointCommandTRB*) xhci_alloc_command_trb(session);
    trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE; // Cycle Bit instellen
    trb->TRBType = XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE; // CONGIGURE ENDPOINT Command TRB Type
    trb->SlotID = device->slot_id; // Slot ID van het apparaat inst
    trb->DataBufferPointerLo = (uint32_t)(uint64_t)(device->infostructures);
	trb->DataBufferPointerHi = (uint32_t)0;

    xhci_thingdong(session, device, (void*)trb, 0, 0);

}