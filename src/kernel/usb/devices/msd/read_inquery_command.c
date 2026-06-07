#include "xhci.h"

void read_inquery_command(XHCIControllerSession *session, USBDevice* device){
    device->initialisation_status=6;
    void* data = kalloc();
    xhci_recieve_bulk(session,device,SCSI_INQUIRY_LENGTH,data);
}