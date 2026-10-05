#include "xhci.h"

void* msd_load_file(XHCIControllerSession *session, USBDevice* device,fat32_file_entry* bestand){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    uint32_t first_data_sector = msd->vbr->RsvdSecCnt + (msd->vbr->NumFATs * msd->vbr->FATSz32);
    uint32_t cluster = (bestand->FstClusHI << 16) | bestand->FstClusLO;
    uint32_t sector = ((cluster - 2) * msd->vbr->SecPerClus) + first_data_sector;
    
    msd->target=3;
    
    printk("-> base %d sector %d <hi %d lo %d> name %s [ blocks %d : %d ]\n",msd->mbr->part1.lba,sector,bestand->FstClusHI,bestand->FstClusLO,bestand->Name,(bestand->FileSize/SCSI_SECTOR_SIZE)+1,bestand->FileSize);
    
    void* primairybuffer = kalloc();
    define_linear_memory_block(primairybuffer,0);
    // for(uint32_t i = MEMORY_PAGE_SIZE ; i < bestand->FileSize ; i += MEMORY_PAGE_SIZE)
    // {
    //     void* c = kalloc();
    //     define_linear_memory_block(c,0);
    // }
    
    uint32_t z = 0;
    uint32_t sectortransfersize = 8;
    uint32_t transfersize = sectortransfersize*SCSI_SECTOR_SIZE;
    for(uint32_t i = 0 ; i < bestand->FileSize ; i += transfersize)
    {
        uint32_t lba = sector + z + msd->mbr->part1.lba;
        printk("Loading %d bytes at offset %d out of %d \n",transfersize,lba,bestand->FileSize/SCSI_SECTOR_SIZE);
        
        msd_read_sector(session,device,lba,transfersize/SCSI_SECTOR_SIZE);
        
        #ifndef XHCI_XHCI_TREAD
        xhci_keep_running = 1;
        event_watcher(1000);
        #else 
        while(msd->file_load_is_ready==0){xhci_check_event();}
        #endif

        memcpy((void*)(uint64_t)(((uint64_t)primairybuffer)+(i*transfersize)),msd->filebuffer,transfersize);

        z+=sectortransfersize;
    }

    return primairybuffer;

}