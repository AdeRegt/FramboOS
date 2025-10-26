#include "bootloader.h"
#include "beeldscherm.h"
#include "geheugen.h"

void kernel_main(BootInfo *bootinfo)
{

    //
    // beelscherm initialiseren
    beeldscherm_laad(bootinfo->graphics_info);

    //
    // geheugen initialiseren
    geheugen_initialiseer(bootinfo->memory_info);
    
    while (1)
    {
        // Kernel loop
    }
}