#include "xhci.h"

void xhci_check_event()
{
    for(int sessionid = 0 ; sessionid < xhci_session_count; sessionid++){
        XHCIControllerSession *session = &xhci_session[sessionid];
        volatile uint32_t *event_ring = session->xhci_event_ring;
        for(int c = 0 ; c < XHCI_EVENT_RING_SIZE ; c++){
            volatile uint32_t wingA = event_ring[c*4 + 0];
            volatile uint32_t wingB = event_ring[c*4 + 1];
            volatile uint32_t wingC = event_ring[c*4 + 2];
            volatile uint32_t wingD = event_ring[c*4 + 3];
            if( (wingD & 0x1) == XHCI_CRCS_DEFAULT_CYCLE_STATE ){
                // Er is een nieuw event
                uint8_t event_type = (wingD >> 10) & 0x3F;
                switch(event_type){
                    case XHCI_TRB_TRANSFER_EVENT_TRB_TYPE: // Transfer Event
                        TransferEventTRB* te_event = (TransferEventTRB*)&event_ring[c*4];
                        xhci_handle_transfer_event(session, te_event);
                        break;
                    case XHCI_TRB_COMMAND_COMPLETION_EVENT_TRB_TYPE: // Command Completion Event
                        CommandCompletionEventTRB* cc_event = (CommandCompletionEventTRB*)&event_ring[c*4];
                        xhci_handle_command_completion_event(session, cc_event);
                        break;
                    case XHCI_TRB_PORT_STATUS_CHANGE_EVENT_TRB_TYPE: // Port Status Change Event
                        PortStatusChangeEventTransferRequestBlock* psc_event = (PortStatusChangeEventTransferRequestBlock*)&event_ring[c*4];
                        xhci_handle_port_change_event(session, psc_event);
                        break;
                    default:
                        printk("XHCI EW: Onbekend Event Type %d gedetecteerd\n", event_type);
                        break;
                }
                // Markeer dit event als verwerkt door de cycle bit te toggelen
                wingD ^= 0x1;
                event_ring[c*4 + 3] = wingD;
            }
        }
    }
}