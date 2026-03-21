#include "xhci.h"

void msd_read_sector(XHCIControllerSession *session, USBDevice* device,uint8_t lba,uint8_t length){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    cbw* cb = create_scsi_command();
    cb->dCBWDataTransferLength = 512*length;
    cb->bmCBWFlags = USB_DIR_IN;
    cb->bCBWCBLength = SCSI_CBW_LENGTH;
    cb->CBWCB[0] = SCSI_READ;
    cb->CBWCB[1] = 0x00;
    cb->CBWCB[2] = 0x00;
    cb->CBWCB[3] = 0x00;
    cb->CBWCB[4] = 0x00;
    cb->CBWCB[5] = lba;// loc
    cb->CBWCB[6] = 0x00;
    cb->CBWCB[7] = 0x00;
    cb->CBWCB[8] = length;
    cb->CBWCB[9] = 0x00;

    msd->loop_id=1;
    msd->expected_sector_count = length;

    xhci_send_bulk(session,device,sizeof(cbw),cb);
}