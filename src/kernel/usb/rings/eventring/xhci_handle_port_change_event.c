#include "xhci.h"

#define RW1C_MASK 0xFE0002

void xhci_handle_port_change_event(XHCIControllerSession *session, PortStatusChangeEventTransferRequestBlock* psc_event)
{
    uint8_t port_id = psc_event->PortID;
    uint8_t completion_code = psc_event->CompletionCode;
    uint8_t calculatedportid = port_id - 1; // Poorten zijn 0-gebaseerd in de registers
    // printk("==> %x \n",PORTSC(calculatedportid));
    // if(PORTSC_PED(calculatedportid)==0){
    //     // de poort is uitgeschkeld
    //     // we kunnen hem inschakelen 
    //     PORTSC(calculatedportid) |= 0b00000000000000000000000000010000; // Poort resetten
    //     return;
    // }
    uint32_t portsc = PORTSC(calculatedportid);
    uint32_t rw1c_mask = (1 << 1) | (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20) | (1 << 21) | (1 << 22) | (1 << 23);

    // Wis al deze statusbits in onze lokale variabele (zodat we ze niet per ongeluk wissen in de hardware)
    portsc &= ~rw1c_mask;

    // Zorg er ook voor dat bit 16 (LWS - Link State Write Strobe) op 0 staat, 
    // zodat we niet per ongeluk de link-state veranderen.
    portsc &= ~(1 << 16);

    // 3. Zet NU alleen de CSC (bit 17) op 1 om deze specifiek te acknowledgen
    portsc |= (1 << 17);
    PORTSC(calculatedportid) = portsc;
    // PORTSC(calculatedportid) |= 0x200000;
    for(int i = 0 ; i < 10000000 ; i++);
    portsc = PORTSC(calculatedportid);
    portsc &= ~RW1C_MASK;
    portsc |= (1 << 4);
    PORTSC(calculatedportid) = portsc;
    while(1){
        portsc = PORTSC(calculatedportid);
        if((portsc & (1 << 4))==0){
            break;
        }
    }
    portsc = PORTSC(calculatedportid);
    portsc &= ~RW1C_MASK;
    portsc |= (1 << 20);
    PORTSC(calculatedportid) = portsc;

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