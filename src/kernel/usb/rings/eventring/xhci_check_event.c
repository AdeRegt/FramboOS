#include "xhci.h"

void xhci_check_event()
{
    for(int sessionid = 0 ; sessionid < xhci_session_count; sessionid++){
        XHCIControllerSession *session = &xhci_session[sessionid];
        volatile uint32_t *event_ring = (volatile uint32_t*)(ERDP_L(0) & 0xFFFFFFF0);
        // for(int c = 0 ; c < XHCI_EVENT_RING_SIZE ; c++){
            volatile uint32_t wingA = event_ring[0];
            volatile uint32_t wingB = event_ring[1];
            volatile uint32_t wingC = event_ring[2];
            volatile uint32_t wingD = event_ring[3];
            if( (wingD & 0x1) == session->xhci_event_ring_cycle_state ){
                // Er is een nieuw event
                uint8_t event_type = (wingD >> 10) & 0x3F;
                switch(event_type){
                    case XHCI_TRB_TRANSFER_EVENT_TRB_TYPE: // Transfer Event
                        TransferEventTRB* te_event = (TransferEventTRB*)&event_ring[0];
                        xhci_handle_transfer_event(session, te_event);
                        break;
                    case XHCI_TRB_COMMAND_COMPLETION_EVENT_TRB_TYPE: // Command Completion Event
                        CommandCompletionEventTRB* cc_event = (CommandCompletionEventTRB*)&event_ring[0];
                        xhci_handle_command_completion_event(session, cc_event);
                        break;
                    case XHCI_TRB_PORT_STATUS_CHANGE_EVENT_TRB_TYPE: // Port Status Change Event
                        PortStatusChangeEventTransferRequestBlock* psc_event = (PortStatusChangeEventTransferRequestBlock*)&event_ring[0];
                        xhci_handle_port_change_event(session, psc_event);
                        break;
                    default:
                        printk("XHCI EW: Onbekend Event Type %d gedetecteerd\n", event_type);
                        break;
                }
                // Markeer dit event als verwerkt door de cycle bit te toggelen
                // wingD ^= 0x1;
                // event_ring[3] = wingD;
                // Markeer in de eventring
                
                uint32_t d = ((ERDP_L(0) & 0xFFFFFFF0)-((uint64_t)session->xhci_event_ring));
                if(d){
                    d /= 0x10;
                }
                if(d==(XHCI_EVENT_RING_SIZE-1)){
                    // printk("XHCI: warning: eventring spoiled!\n");
                    session->xhci_event_ring_cycle_state ^= 1;
                    // memset((void*)event_ring,0,(sizeof(uint32_t)*4)*XHCI_EVENT_RING_SIZE);
                    ERDP_L(0) = (uint64_t)(uintptr_t)session->xhci_event_ring | 1;
                    ERDP_H(0) = 0;
                }else{
                    ERDP_L(0) = (uint64_t)((uintptr_t)&event_ring[4]) | 1;
                    ERDP_H(0) = 0;
                }
                // printk("HALTED HERE");
                // for(;;);
            }
        // }
    }
}