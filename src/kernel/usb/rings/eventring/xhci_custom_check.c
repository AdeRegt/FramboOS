#include "xhci.h"

void xhci_custom_check(XHCIControllerSession *session)
{
    for(int pid = 0 ; pid < HCSPARAMS1_MaxPorts ; pid++){
        if(PORTSC(pid)&3==3){
            session->devices[session->max_ports].physical_port_id = pid;
            xhci_send_enable_slot(session,(USBDevice*) &session->devices[session->max_ports]);
            session->max_ports++;
        }
    }
}