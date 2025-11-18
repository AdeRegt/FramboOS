#include "xhci.h"

void xhci_handle_transfer_event(XHCIControllerSession *session, TransferEventTRB* transfer_event)
{
    USBDevice* thisdevice = NULL;
    uint8_t old_trb_type = 0;
    for(int i = 0 ; i < session->max_ports ; i++){
        USBDevice* device = &session->devices[i];
        if(device->pointer_to_requested_trb != NULL){
            if( (uint32_t)((uint64_t)device->pointer_to_requested_trb) == transfer_event->DataBufferPointerLo ){
                // Dit is het apparaat waarvoor we op een transfer event wachten
                old_trb_type = (((uint32_t*)device->pointer_to_requested_trb)[3] >> 10) & 0x3F;
                device->pointer_to_requested_trb = NULL;
                thisdevice = device;
                break;
            }
        }
    }
    //
    // Hier komt de transfer event handling code
    //
    if(transfer_event->CompletionCode == 1){ // Succes
        switch(old_trb_type){
            case 4: // Status Stage TRB
                printk("XHCI TE: Transfer succesvol voltooid voor apparaat op poort %d, Slot ID: %d\n", thisdevice->physical_port_id + 1, thisdevice->slot_id);
                
                break;
            default:
                printk("XHCI TE: Onbekend TRB Type %d succesvol voltooid voor apparaat op poort %d\n", old_trb_type, thisdevice->physical_port_id + 1);
                break;
        }
    } else {
        printk("XHCI TE: Fout TRB Type %d voor apparaat op port %d, Completion Code: %s\n", old_trb_type, thisdevice->physical_port_id + 1, xhci_get_resultcode_string(transfer_event->CompletionCode));
    }
}