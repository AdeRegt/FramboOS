#include "bestandensysteem.h"
#include "xhci.h"

void* bestand(uint8_t* path){
    uint8_t* pth = (uint8_t*) alloc_page();
    MSDDevice* f = (MSDDevice*) fshook;
    MassStorageDevice* msd = (MassStorageDevice*) f->device->attachment;

    fat32_file_entry* bst = (fat32_file_entry*) NULL;
    for(int i = 0 ; i < 16 ; i++){
        fat32_file_entry file = msd->root_directory->files[i];
        int h = 0;
        if(file.Attr==0x20){
            for(int z = 0 ; z < 11-3 ; z++){
                if(file.Name[z]!=' '){
                    if(file.Name[z]!=path[h]){
                        goto nogmaals;
                    }
                    h++;
                }
            }
            if(path[h]!='.'){
                goto nogmaals;
            }
            h++;
            for(int z = 0 ; z < 3 ; z++){
                if(file.Name[(11-3)+z]!=' '){
                    if(file.Name[(11-3)+z]!=path[h]){
                        goto nogmaals;
                    }
                    h++;
                }
            }
            bst = (fat32_file_entry*) &msd->root_directory->files[i];
        }
        nogmaals:
    }
    if(!bst){
        return NULL;
    }
    return msd_load_file(f->session,f->device,bst);
}