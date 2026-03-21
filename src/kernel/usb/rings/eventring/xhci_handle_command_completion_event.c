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
            case XHCI_TRB_ENABLE_SLOT_COMMAND_TRB_TYPE: // Enable Slot Command TRB
                printk("XHCI CCE: Enable Slot Command succesvol voltooid voor apparaat op poort %d, toegewezen Slot ID: %d\n", thisdevice->physical_port_id + 1, cc_event->SlotID);
                xhci_send_set_address(session, thisdevice);
                break;
            case XHCI_TRB_SET_ADDRESS_COMMAND_TRB_TYPE: // Address Device Command TRB
                printk("XHCI CCE: Address Device Command succesvol voltooid voor apparaat op port %d .\n", thisdevice->physical_port_id + 1);
                xhci_send_request_device_descriptor(session, thisdevice);
                break;
            case XHCI_TRB_CONFIGURE_ENDPOINT_COMMAND_TRB_TYPE: // Configure Endpoint Command TRB
                printk("XHCI CCE: Configure Endpoint Command succesvol voltooid voor apparaat op poort %d .\n", thisdevice->physical_port_id + 1);
                xhci_device_event_router(session, thisdevice);
                break;
            default:
                printk("XHCI CCE: Onbekend TRB Type %d succesvol voltooid voor apparaat op poort %d\n", old_trb_type, thisdevice->physical_port_id + 1);
                break;
        }
    } else {
        printk("XHCI CCE: Fout %s voor apparaat op poort %d, Completion Code: %s\n", xhci_trb_type_to_string(old_trb_type), thisdevice->physical_port_id + 1, xhci_get_resultcode_string(cc_event->CompletionCode));
    }
}