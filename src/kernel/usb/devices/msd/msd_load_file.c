#include "xhci.h"

void* msd_load_file(XHCIControllerSession *session, USBDevice* device,fat32_file_entry* bestand){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    uint32_t first_data_sector = msd->vbr->RsvdSecCnt + (msd->vbr->NumFATs * msd->vbr->FATSz32);
    uint32_t cluster = (bestand->FstClusHI << 16) | bestand->FstClusLO;
    uint32_t sector = ((cluster - 2) * msd->vbr->SecPerClus) + first_data_sector;
    
    msd->target=3;
    
    printk("-> %d <%d %d> %s [ %d ]\n",sector,bestand->FstClusHI,bestand->FstClusLO,bestand->Name,(bestand->FileSize/512)+1);
    msd_read_sector(session,device,sector,(bestand->FileSize/512)+1);

    #ifndef XHCI_XHCI_TREAD
    xhci_keep_running = 1;
    event_watcher();
    #endif

    return msd->filebuffer;

}