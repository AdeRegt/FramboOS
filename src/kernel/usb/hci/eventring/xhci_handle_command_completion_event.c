#include "xhci.h"

void xhci_handle_command_completion_event(XHCIControllerSession *session, CommandCompletionEventTRB* cc_event)
{
    USBDevice* thisdevice = NULL;
    uint8_t old_trb_type = 0;
    for(int i = 0 ; i < session->max_ports ; i++){
        USBDevice* device = &session->devices[i];
        if(device->pointer_to_requested_trb != NULL){
            if( (uint32_t)((uint64_t)device->pointer_to_requested_trb) == cc_event->DataBufferPointerLo ){
                // Dit is het apparaat waarvoor we op een command completion event wachten
                old_trb_type = (((uint32_t*)device->pointer_to_requested_trb)[3] >> 10) & 0x3F;
                device->pointer_to_requested_trb = NULL;
                thisdevice = device;
                break;
            }
        }
    }
    //
    // Hier komt de command completion event handling code
    //
    thisdevice->slot_id = cc_event->SlotID;
    if(cc_event->CompletionCode == 1){ // Succes
        switch(old_trb_type){
            case 9: // Enable Slot Command TRB
                printk("XHCI Command Completion Event: Enable Slot Command succesvol voltooid voor apparaat op poort %d, toegewezen Slot ID: %d\n", thisdevice->physical_port_id + 1, cc_event->SlotID);
                break;
            default:
                printk("XHCI Command Completion Event: Onbekend TRB Type %d succesvol voltooid voor apparaat op poort %d\n", old_trb_type, thisdevice->physical_port_id + 1);
                break;
        }
    } else {
        printk("XHCI Command Completion Event: Fout bij uitvoeren van command TRB Type %d voor apparaat op poort %d, Completion Code: %d\n", old_trb_type, thisdevice->physical_port_id + 1, cc_event->CompletionCode);
    }
}