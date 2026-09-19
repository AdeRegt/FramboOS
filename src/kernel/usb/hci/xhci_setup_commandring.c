#include "xhci.h"

void xhci_setup_commandring(XHCIControllerSession *session)
{
    session->xhci_command_ring = (uint32_t*) kalloc();
    define_linear_memory_block((void*)(uintptr_t)(session->xhci_command_ring),1);

    //
    // Initialiseer de Command Ring
    //
    for (int i = 0; i < XHCI_COMMAND_RING_SIZE*4; i++)
    {
        session->xhci_command_ring[i] = 0;
    }

    uintptr_t ring_addr = (uintptr_t)&session->xhci_command_ring[0];
    printd("DEBUG: Adres van command ring = %x (aligned? %d)\n", ring_addr, (ring_addr & 0x3F) == 0);
    
    // Controleer of de controller wel echt halted is
    printd("DEBUG voor schrijven CRCR - USBSTS HCH = %d\n", (USBSTS & 1));
    while (USBSTS & (1 << 11)) {
        // Wacht tot CNR (Controller Not Ready) verdwijnt
    }
//     printd("DEBUG VÓÓR CRCR: USBSTS=%x (HCH=%d, CNR=%d), CAPLENGTH=%d, ring=%x\n", 
//        USBSTS, (USBSTS & 1), (USBSTS & (1 << 11)) >> 11, CAPLENGTH, &session->xhci_command_ring[0]);
//     uintptr_t ring_addrx = (uintptr_t)&session->xhci_command_ring[0];
// CRCR_64 = (uint64_t)ring_addrx | XHCI_CRCS_DEFAULT_CYCLE_STATE;
//     define_linear_memory_block((void*)(uintptr_t)(ring_addrx),1);

// printd("xhci_setup_commandring: Command Ring ingesteld op %x (64-bit)\n", CRCR_64);
//     // CRCR_L = (uint64_t)ring_addr | XHCI_CRCS_DEFAULT_CYCLE_STATE;
//     // CRCR_H = (uint64_t)(ring_addr >> 32); // Belangrijk voor 64-bit systemen!

//     printd("xhci_setup_commandring: Na schrijven CRCR_L=%x, CRCR_H=%x\n", CRCR_L, CRCR_H);
//     return;
//     // for(;;);

    //
    // Stel de Command Ring Pointer in
    //
    CRCR_L = (uint64_t)(uintptr_t)&session->xhci_command_ring[0] | XHCI_CRCS_DEFAULT_CYCLE_STATE;
    CRCR_H = 0;

    //
    // Stel de Command Ring Cycle State in
    //
    // CRCR_L |= XHCI_CRCS_DEFAULT_CYCLE_STATE;

    printd("xhci_setup_commandring: Command Ring ingesteld op %x met cyclestate %d \n", CRCR_L, XHCI_CRCS_DEFAULT_CYCLE_STATE);
}