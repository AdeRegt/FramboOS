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
        sleep(100);
        while(PORTSC_PR(calculatedportid))
        {
            sleep(100);
        }
        while(PORTSC_PED(calculatedportid)==0)
        {
            sleep(100);
        }
        return;
    }

    switch(completion_code) {
        case 1: // Success
            printk("XHCI Handle Port Change Event: Poort %d succesvol gewijzigd.", port_id);
            break;
        case 2: // Port Disabled
            printk("XHCI Handle Port Change Event: Poort %d is uitgeschakeld.", port_id);
            break;
        case 3: // Connect Status Changed
            printk("XHCI Handle Port Change Event: Verbindingsstatus van poort %d is gewijzigd.", port_id);
            break;
        default:
            printk("XHCI Handle Port Change Event: Onbekende completion code %d voor poort %d.", completion_code, port_id);
            break;
    }
    printk(" PORTSC=%x \n",PORTSC(calculatedportid));
}