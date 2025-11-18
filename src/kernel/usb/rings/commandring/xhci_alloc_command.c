#include "xhci.h"

void* xhci_alloc_command_trb(XHCIControllerSession *session)
{
    // Allocate a new command TRB from the command ring
    void* trb = (void*) &session->xhci_command_ring[session->command_ring_index];
    memset(trb, 0, sizeof(EnableSlotCommandTRB)); // Clear the TRB memory
    session->command_ring_index += 4; // Move to the next TRB (4 DWORDs per TRB)
    return trb;
}