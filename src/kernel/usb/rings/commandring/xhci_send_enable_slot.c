#include "xhci.h"

void xhci_send_enable_slot(XHCIControllerSession *session, USBDevice* device)
{
    // Reset command ring index or pointer as needed
    EnableSlotCommandTRB *enable_slot_trb = (EnableSlotCommandTRB*) xhci_alloc_command_trb(session);
    enable_slot_trb->CycleBit = XHCI_CRCS_DEFAULT_CYCLE_STATE;
    enable_slot_trb->TRBType = XHCI_TRB_ENABLE_SLOT_COMMAND_TRB_TYPE; // Enable Slot Command TRB
    enable_slot_trb->SlotType = 0; // Device Slot
    
    xhci_thingdong(session, device, (void*)enable_slot_trb, 0, 0);

}