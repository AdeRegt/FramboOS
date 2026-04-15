#include "xhci.h"
#include "bestandensysteem.h"

void msd_router(XHCIControllerSession *session, USBDevice* device, TransferTRB* transfer_event){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    if(msd->loop_id==1){
        void* data = alloc_page();
        xhci_recieve_bulk(session,device,msd->datalength,data);
        msd->loop_id=2;
        return;
    }
    if(msd->loop_id==3){
        uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
        csw* c = (csw*) inc;
        printk("scsi response: status: %d residue: %d \n",c->bCSWStatus,c->dCSWDataResidue);
    }
    if(msd->target==0){
        if(msd->loop_id==0){
            msd_read_sector(session,device,0,1);
        }else if(msd->loop_id==2){
            uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
            msd->mbr = (master_boot_record*)inc;
            if(!(msd->mbr->bootsignature[0]==0x55&&msd->mbr->bootsignature[1]==0xAA)){
                printk("Strange disk format\n");
            }
            if(msd->mbr->part1.type!=0xB){
                printk("Invalid filesystemformat\n");
            }
            // printk("1 lba:%x type:%x length:%x \n",msd->mbr->part1.lba,msd->mbr->part1.type,msd->mbr->part1.length);
            printk("USB STICK: FAT32 filesystem found at LBA %d \n",msd->mbr->part1.lba);
            msd->lba_offset = msd->mbr->part1.lba;
        }else if(msd->loop_id==3){
            // printk("read sectors has now a standard offset of %d \n",msd->lba_offset);
            msd->target = 1;
            msd_read_sector(session,device,0,1);
        }
    }
    else if(msd->target==1){
        if(msd->loop_id==2){
            uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
            msd->vbr = (fat32_volume_boot_record*) inc;
        }else if(msd->loop_id==3){
            // printk("read sectors has now a standard offset of %d \n",msd->lba_offset);
            msd->target = 2;
            uint32_t first_data_sector = msd->vbr->RsvdSecCnt + (msd->vbr->NumFATs * msd->vbr->FATSz32);
            uint32_t cluster = msd->vbr->RootClus;
            uint32_t sector = ((cluster - 2) * msd->vbr->SecPerClus) + first_data_sector;
            // printk("FAT root directory LBA at %d \n",sector);
            msd_read_sector(session,device,sector,1);
        }
    }
    else if(msd->target==2){
        if(msd->loop_id==2){
            uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
            msd->root_directory = (fat32_directory*) inc;
        }else if(msd->loop_id==3){
            
            MSDDevice* dev = (MSDDevice*) alloc_page();
            dev->device = device;
            dev->session = session;
            bs_regristreer(dev);

            #ifndef XHCI_XHCI_TREAD
            xhci_keep_running = 0;
            #endif 

        }
    }
    else if(msd->target==3){
        if(msd->loop_id==2){
            uint8_t* inc = (uint8_t*) (uint64_t)transfer_event->DataBufferPointerLo;
            msd->filebuffer = inc;
        }else if(msd->loop_id==3){
            msd->file_load_is_ready = 1;
            // printk("done loading file\n");

            #ifndef XHCI_XHCI_TREAD
            xhci_keep_running = 0;
            #endif 
        }
    }
    if(msd->loop_id==2){
        msd->loop_id=3;
        void* data = alloc_page();
        xhci_recieve_bulk(session,device,13,data);
    }
}