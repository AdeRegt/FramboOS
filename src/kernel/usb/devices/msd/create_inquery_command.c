#include "xhci.h"

void create_inquery_command(XHCIControllerSession *session, USBDevice* device){
    cbw* cb = create_scsi_command();
    cb->dCBWDataTransferLength = 36;
    cb->bmCBWFlags = USB_DIR_IN;
    cb->bCBWCBLength = 6;
    cb->CBWCB[0] = SCSI_INQUIRY;
    cb->CBWCB[1] = 0x00;
    cb->CBWCB[2] = 0x00;
    cb->CBWCB[3] = 0x00;
    cb->CBWCB[4] = 36;
    cb->CBWCB[5] = 0x00;

    xhci_send_bulk(session,device,sizeof(cbw),cb);
}