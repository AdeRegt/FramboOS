#include "xhci.h"

void create_inquery_command(XHCIControllerSession *session, USBDevice* device){
    cbw* cb = create_scsi_command();
    cb->dCBWDataTransferLength = SCSI_INQUIRY_LENGTH;
    cb->bmCBWFlags = USB_DIR_IN;
    cb->bCBWCBLength = SCSI_CBW_LENGTH;
    cb->CBWCB[0] = SCSI_INQUIRY;
    cb->CBWCB[1] = 0x00;
    cb->CBWCB[2] = 0x00;
    cb->CBWCB[3] = 0x00;
    cb->CBWCB[4] = SCSI_INQUIRY_LENGTH;
    cb->CBWCB[5] = 0x00;

    device->initialisation_status = 5;

    xhci_send_bulk(session,device,sizeof(cbw),cb);
}