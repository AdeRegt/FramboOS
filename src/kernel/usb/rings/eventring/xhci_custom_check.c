#include "xhci.h"

uint8_t xhci_custom_check(XHCIControllerSession *session)
{
    uint8_t res = 0;
    for(int pid = 0 ; pid < HCSPARAMS1_MaxPorts ; pid++)
    {
        
        if ((PORTSC(pid) & 3) == 3)
        {
            printk("--> PORT DETECTED\n");
            uint32_t port_val = PORTSC(pid);
            PORTSC(pid) = (port_val & 0x0001D101) | (1 << 17);
            
            session->devices[session->max_ports].physical_port_id = pid;
            xhci_send_enable_slot(session,(USBDevice*) &session->devices[session->max_ports]);
            session->max_ports++;
            res++;
        }
        else
        {
            PORTSC(pid) |= (1 << 9);
        }
    }
    return res;
}