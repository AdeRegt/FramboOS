#include "xhci.h"

void event_watcher()
{
    //
    // Hier komt de event watcher code
    //
    again:
        //
        // Controleer de Event Ring op nieuwe events
        //
        sleep(100);
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
                        case 32: // Transfer Event
                            printk("XHCI Event Watcher: Transfer Event gedetecteerd\n");
                            break;
                        case 33: // Command Completion Event
                            CommandCompletionEventTRB* cc_event = (CommandCompletionEventTRB*)&event_ring[c*4];
                            xhci_handle_command_completion_event(session, cc_event);
                            break;
                        case 34: // Port Status Change Event
                            PortStatusChangeEventTransferRequestBlock* psc_event = (PortStatusChangeEventTransferRequestBlock*)&event_ring[c*4];
                            xhci_handle_port_change_event(session, psc_event);
                            break;
                        default:
                            printk("XHCI Event Watcher: Onbekend Event Type %d gedetecteerd\n", event_type);
                            break;
                    }
                    // Markeer dit event als verwerkt door de cycle bit te toggelen
                    wingD ^= 0x1;
                    event_ring[c*4 + 3] = wingD;
                }
            }
        }
    goto again;
}