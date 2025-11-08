#include "xhci.h"

void xhci_set_max_ports()
{
    //
    // Maximale poorten instellen 
    //
    uint8_t max_ports = HCSPARAMS1_MaxPorts;
    printk("XHCI meld dat er %d poorten zijn die we kunnen gebruiken.\n", max_ports);

    CONFIG |= max_ports;
}