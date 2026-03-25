#include "xhci.h"

uint8_t xhci_check_for_new_devs()
{
    for(int sessionid = 0 ; sessionid < xhci_session_count; sessionid++)
    {
        XHCIControllerSession *session = &xhci_session[sessionid];
        volatile uint32_t *event_ring = session->xhci_event_ring;
        for(int c = 0 ; c < XHCI_EVENT_RING_SIZE ; c++)
        {
            volatile uint32_t wingD = event_ring[c*4 + 3];
            if( (wingD & 0x1) == XHCI_CRCS_DEFAULT_CYCLE_STATE )
            {
                // Er is een nieuw event
                uint8_t event_type = (wingD >> 10) & 0x3F;
                if(event_type==XHCI_TRB_PORT_STATUS_CHANGE_EVENT_TRB_TYPE)
                {
                    PortStatusChangeEventTransferRequestBlock* psc_event = (PortStatusChangeEventTransferRequestBlock*)&event_ring[c*4];
                    return 1;
                }
            }
        }
    }
    return 0;
}