#include "xhci.h"

void* msd_load_file(XHCIControllerSession *session, USBDevice* device,fat32_file_entry* bestand){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    uint32_t first_data_sector = msd->vbr->RsvdSecCnt + (msd->vbr->NumFATs * msd->vbr->FATSz32);
    uint32_t cluster = (bestand->FstClusHI << 16) | bestand->FstClusLO;
    uint32_t sector = ((cluster - 2) * msd->vbr->SecPerClus) + first_data_sector;
    
    msd->target=3;
    
    printk("-> %d <%d %d> %s [ %d ]\n",sector,bestand->FstClusHI,bestand->FstClusLO,bestand->Name,(bestand->FileSize/SCSI_SECTOR_SIZE)+1);
    
    void* primairybuffer = alloc_page();
    for(uint32_t i = MEMORY_PAGE_SIZE ; i < bestand->FileSize ; i += MEMORY_PAGE_SIZE)
    {
        alloc_page();
    }
    
    uint32_t z = 0;
    uint32_t sectortransfersize = 8;
    uint32_t transfersize = sectortransfersize*SCSI_SECTOR_SIZE;
    for(uint32_t i = 0 ; i < bestand->FileSize ; i += transfersize)
    {
        printk("Loading %d bytes at offset %d out of %d \n",transfersize,z,bestand->FileSize/SCSI_SECTOR_SIZE);
        
        msd_read_sector(session,device,sector + z,transfersize/SCSI_SECTOR_SIZE);

        #ifndef XHCI_XHCI_TREAD
        xhci_keep_running = 1;
        event_watcher();
        #endif

        memcpy((void*)(uint64_t)(((uint64_t)primairybuffer)+(i*transfersize)),msd->filebuffer,transfersize);

        z+=sectortransfersize;
    }

    return primairybuffer;

}