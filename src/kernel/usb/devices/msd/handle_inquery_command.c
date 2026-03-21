#include "xhci.h"

void handle_inquery_command(XHCIControllerSession *session, USBDevice* device, TransferTRB* transfer_event){
    inquiry_response* inc = (inquiry_response*) (uint64_t)transfer_event->DataBufferPointerLo;
    printk("USB STICK ");
    for(int i = 0 ; i < 8 ; i++){
        printk("%c",inc->vendor_id[i]);
    }
    printk(" ");
    for(int i = 0 ; i < 16 ; i++){
        printk("%c",inc->product_id[i]);
    }
    printk(" ");
    for(int i = 0 ; i < 4 ; i++){
        printk("%c",inc->product_revision[i]);
    }
    printk(" \n");

    device->initialisation_status = 7;

    device->devicetype = 1;

    MassStorageDevice* msd = (MassStorageDevice*) alloc_page();
    msd->inquery = inc;

    device->attachment = msd;

    void* data = alloc_page();
    xhci_recieve_bulk(session,device,13,data);
}