#include "xhci.h"

void xhci_send_enable_slot(XHCIControllerSession *session, USBDevice* device)
{
    // Reset command ring index or pointer as needed
    EnableSlotCommandTRB *enable_slot_trb = (EnableSlotCommandTRB*) &session->xhci_command_ring[session->command_ring_index];
    enable_slot_trb->rsvrd1 = 0;
    enable_slot_trb->rsvrd2 = 0;
    enable_slot_trb->rsvrd3 = 0;
    enable_slot_trb->CycleBit = 1;
    enable_slot_trb->RsvdZ1 = 0;
    enable_slot_trb->TRBType = XHCI_TRB_ENABLE_SLOT_COMMAND_TRB_TYPE; // Enable Slot Command TRB
    enable_slot_trb->SlotType = 0; // Device Slot
    enable_slot_trb->RsvdZ2 = 0;
    session->command_ring_index += 4; // Move to the next TRB (4 DWORDs per TRB)
    
    xhci_thingdong(session, device, (void*)enable_slot_trb);

}