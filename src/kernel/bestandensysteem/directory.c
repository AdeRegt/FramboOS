#include "bestandensysteem.h"
#include "xhci.h"

uint8_t* directory(){
    uint8_t* pth = (uint8_t*) alloc_page();
    MSDDevice* f = (MSDDevice*) fshook;
    MassStorageDevice* msd = (MassStorageDevice*) f->device->attachment;

    int pointer = 0;
    int ppp = 0;

    for(int i = 0 ; i < 16 ; i++){
        fat32_file_entry file = msd->root_directory->files[i];
        if(file.Attr==0x20){
            if(ppp!=0){
                pth[pointer++] = '|';    
            }
            for(int z = 0 ; z < 11-3 ; z++){
                if(file.Name[z]!=' '){
                    pth[pointer++] = file.Name[z];
                }
            }
            pth[pointer++] = '.';
            for(int z = 0 ; z < 3 ; z++){
                if(file.Name[(11-3)+z]!=' '){
                    pth[pointer++] = file.Name[(11-3)+z];
                }
            }
            ppp++;
        }
    }
    pth[pointer++] = 0x00;

    return pth;
}