#include "xhci.h"

void xhci_handle_port_change_event(XHCIControllerSession *session, PortStatusChangeEventTransferRequestBlock* psc_event)
{
    uint8_t port_id = psc_event->PortID;
    uint8_t completion_code = psc_event->CompletionCode;
    uint8_t calculatedportid = port_id - 1; // Poorten zijn 0-gebaseerd in de registers

    if(PORTSC_PED(calculatedportid)==0){
        // de poort is uitgeschkeld
        // we kunnen hem inschakelen 
        PORTSC(calculatedportid) |= 0b00000000000000000000000000010000; // Poort resetten
        return;
    }
    PORTSC(calculatedportid) |= 0x200000;

    switch(completion_code) {
        case 1: // Success
            printk("XHCI HPCE: Poort %d succesvol gewijzigd.", port_id);
            break;
        case 2: // Port Disabled
            printk("XHCI HPCE: Poort %d is uitgeschakeld.", port_id);
            break;
        case 3: // Connect Status Changed
            printk("XHCI HPCE: Verbindingsstatus van poort %d is gewijzigd.", port_id);
            break;
        default:
            printk("XHCI HPCE: Onbekende completion code %s voor poort %d.", xhci_get_resultcode_string(completion_code), port_id);
            break;
    }
    printk("\n");

    session->devices[session->max_ports].physical_port_id = calculatedportid;

    xhci_send_enable_slot(session,(USBDevice*) &session->devices[session->max_ports]);

    session->max_ports++;
}