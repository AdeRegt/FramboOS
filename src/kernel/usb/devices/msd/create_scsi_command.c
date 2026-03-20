#include "xhci.h"

uint32_t cbwtag = 0x1000;

cbw* create_scsi_command(){
    cbw* rs = (cbw*) alloc_page();
    rs->dCBWSignature = SCSI_CBW_SIGNATURE;
    rs->dCBWTag = cbwtag++;
    return rs;
}