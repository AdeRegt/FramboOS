#include "xhci.h"

void handle_inquery_status(XHCIControllerSession *session, USBDevice* device, TransferTRB* transfer_event){
    csw* inc = (csw*) (uint64_t)transfer_event->DataBufferPointerLo;
    if(inc->dCSWSignature!=SCSI_CSW_SIGNATURE){
        printk("Illegal scsi signature\n");
    }
    if(inc->bCSWStatus!=0){
        printk("Illegal scsi status\n");
    }
    device->initialisation_status = 8;
    msd_router(session,device,transfer_event);
}