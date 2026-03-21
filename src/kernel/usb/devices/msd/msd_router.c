#include "xhci.h"

void msd_router(XHCIControllerSession *session, USBDevice* device, TransferTRB* transfer_event){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    if(msd->loop_id==1){
        void* data = alloc_page();
        xhci_recieve_bulk(session,device,512,data);
        msd->loop_id=2;
        return;
    }
    else if(msd->target==0){
        if(msd->loop_id==0){
            msd_read_sector(session,device,0,1);
        }else if(msd->loop_id==2){
            uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
            msd->mbr = (master_boot_record*)inc;
            if(!(msd->mbr->bootsignature[0]==0x55&&msd->mbr->bootsignature[1]==0xAA)){
                printk("Strange disk format\n");
            }
            printk("1 lba:%x type:%x length:%x \n",msd->mbr->part1.lba,msd->mbr->part1.type,msd->mbr->part1.length);
        }
    }
    if(msd->loop_id==2){
        msd->loop_id=3;
        void* data = alloc_page();
        xhci_recieve_bulk(session,device,13,data);
    }
}