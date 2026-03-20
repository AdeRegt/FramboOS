#include "xhci.h"

void handle_inquery_command(XHCIControllerSession *session, USBDevice* device, TransferTRB* transfer_event){
    inquiry_response* inc = (inquiry_response*) (uint64_t)transfer_event->DataBufferPointerLo;
    printk("USB STICK vendor_id ");
    for(int i = 0 ; i < 8 ; i++){
        printk("%c",inc->vendor_id[i]);
    }
    printk(" product_id ");
    for(int i = 0 ; i < 16 ; i++){
        printk("%c",inc->product_id[i]);
    }
    printk(" product_revision ");
    for(int i = 0 ; i < 4 ; i++){
        printk("%c",inc->product_revision[i]);
    }
    printk(" \n");for(;;);
}