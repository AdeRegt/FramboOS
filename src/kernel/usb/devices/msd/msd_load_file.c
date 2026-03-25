#include "xhci.h"

void* msd_load_file(XHCIControllerSession *session, USBDevice* device,fat32_file_entry* bestand){
    MassStorageDevice* msd = (MassStorageDevice*) device->attachment;
    
    uint32_t first_data_sector = msd->vbr->RsvdSecCnt + (msd->vbr->NumFATs * msd->vbr->FATSz32);
    uint32_t cluster = msd->vbr->RootClus;
    uint32_t sector = ((cluster - 2) * msd->vbr->SecPerClus) + first_data_sector;
    
    msd->target=3;
    
    printk("-> %d <%d %d> %s \n",sector,bestand->FstClusHI,bestand->FstClusLO,bestand->Name);
    msd_read_sector(session,device,sector,1);

    #ifndef XHCI_XHCI_TREAD
    xhci_keep_running = 1;
    event_watcher();
    #endif

    printk("SSSS");
    return NULL;

}