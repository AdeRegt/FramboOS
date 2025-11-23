#include "xhci.h"

void xhci_send_set_config(XHCIControllerSession *session, USBDevice* device, int config_id)
{
    device->initialisation_status = 3;
    // Stuur een Request Configuration Descriptor Command TRB
    SetupStageTRB* setup = (SetupStageTRB*) xhci_alloc_local_trb(session, device);
    setup->usbcmd.bRequestType = 0; // Device to Host, Standard, Device
    setup->usbcmd.bRequest = 9; // GET_DESCRIPTOR
    setup->usbcmd.wValue = config_id; // Descriptor Type (Configuration) en Descriptor Index (0)
    setup->usbcmd.wIndex = 0; // Language ID (0 voor Configuration Descriptor)
    setup->usbcmd.wLength = 0; // Lengte van de Configuration Descriptor
    setup->TRBTransferLength = 8;
    setup->ImmediateData = 1;
    setup->TRT = 3; // Setup Stage
    setup->Cyclebit = 1;
    // setup->InterruptOnCompletion = 1;
    setup->TRBType = XHCI_TRB_SETUP_TRB_TYPE; // Setup Stage TRB

    StatusStageTRB* status = (StatusStageTRB*) xhci_alloc_local_trb(session, device);
    status->Cyclebit = 1;
    status->InterruptOnCompletion = 1;
    status->TRBType = XHCI_TRB_STATUS_TRB_TYPE; // Status Stage TRB
    status->Direction = 0; // Host to Device

    xhci_ring_trb_ring(session, device, (USBRing*)device->commandring, (void*)status);
}