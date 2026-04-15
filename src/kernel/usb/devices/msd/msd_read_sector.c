#include "xhci.h"

void msd_read_sector(XHCIControllerSession *session, USBDevice* device,uint32_t lba,uint8_t length){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    lba += msd->lba_offset;

    cbw* cb = create_scsi_command();
    cb->dCBWDataTransferLength = SCSI_SECTOR_SIZE*length;
    msd->datalength = SCSI_SECTOR_SIZE*length;
    cb->bmCBWFlags = USB_DIR_IN;
    cb->bCBWCBLength = SCSI_CBW_LENGTH;
    cb->CBWCB[0] = SCSI_READ;
    cb->CBWCB[1] = 0x00; // flags
    cb->CBWCB[2] = (lba >> 24) & 0xFF;
    cb->CBWCB[3] = (lba >> 16) & 0xFF;
    cb->CBWCB[4] = (lba >> 8) & 0xFF;
    cb->CBWCB[5] = lba & 0xFF;// loc
    cb->CBWCB[6] = 0x00;
    cb->CBWCB[7] = 0x00;
    cb->CBWCB[8] = length;
    cb->CBWCB[9] = 0x00;

    msd->loop_id=1;

    xhci_send_bulk(session,device,sizeof(cbw),cb);
}