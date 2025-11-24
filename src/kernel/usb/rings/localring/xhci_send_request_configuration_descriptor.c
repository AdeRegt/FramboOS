#include "xhci.h"

void xhci_send_request_configuration_descriptor(XHCIControllerSession *session, USBDevice* device)
{
    USBStandardConfigurationDescriptor* configdesc = (USBStandardConfigurationDescriptor*) alloc_page();
    device->configdesc = configdesc;
    device->initialisation_status = 2; // In afwachting van Configuration Descriptor
    // Stuur een Request Configuration Descriptor Command TRB
    SetupStageTRB* setup = (SetupStageTRB*) xhci_alloc_local_trb(session, device);
    setup->usbcmd.bRequestType = 0x80; // Device to Host, Standard, Device
    setup->usbcmd.bRequest = XHCI_SETUP_PACKAGE_GET_DESCRIPTOR; // GET_DESCRIPTOR
    setup->usbcmd.wValue = 0x0200; // Descriptor Type (Configuration) en Descriptor Index (0)
    setup->usbcmd.wIndex = 0; // Language ID (0 voor Configuration Descriptor)
    setup->usbcmd.wLength = 0x1000; // Lengte van de Configuration Descriptor
    setup->TRBTransferLength = 8;
    setup->ImmediateData = 1;
    setup->TRT = 3; // Setup Stage
    setup->Cyclebit = 1;
    // setup->InterruptOnCompletion = 1;
    setup->TRBType = XHCI_TRB_SETUP_TRB_TYPE; // Setup Stage TRB

    DataStageTRB* data = (DataStageTRB*) xhci_alloc_local_trb(session, device);
    data->Address1 = (uint32_t)( (uint64_t) configdesc ); // Laag 32 bits van het bufferadres
    data->Address2 = (uint32_t)( ( (uint64_t) configdesc ) >> 32 ); // Hoog 32 bits van het bufferadres
    data->TRBTransferLength = 0x1000;
    data->Direction = 1; // Device to Host
    data->Cyclebit = 1;
    // data->InterruptOnCompletion = 1;
    data->TRBType = XHCI_TRB_DATA_TRB_TYPE; // Data Stage TRB

    StatusStageTRB* status = (StatusStageTRB*) xhci_alloc_local_trb(session, device);
    status->Cyclebit = 1;
    status->InterruptOnCompletion = 1;
    status->TRBType = XHCI_TRB_STATUS_TRB_TYPE; // Status Stage TRB
    status->Direction = 0; // Host to Device

    xhci_ring_trb_ring(session, device, (USBRing*)device->commandring, (void*)status);
}