#include "xhci.h"

void xhci_setup_commandring()
{
    //
    // Initialiseer de Command Ring
    //
    for (int i = 0; i < XHCI_COMMAND_RING_SIZE*4; i++)
    {
        xhci_command_ring[i] = 0;
    }

    //
    // Stel de Command Ring Pointer in
    //
    CRCR_L = (uint64_t)(uintptr_t)xhci_command_ring;
    CRCR_H = 0;

    //
    // Stel de Command Ring Cycle State in
    //
    CRCR_L |= XHCI_CRCS_DEFAULT_CYCLE_STATE;

    printk("xhci_setup_commandring: Command Ring ingesteld op %x met cyclestate %d \n", CRCR_L, XHCI_CRCS_DEFAULT_CYCLE_STATE);
}