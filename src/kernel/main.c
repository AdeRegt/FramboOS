#include "bootloader.h"
#include "beeldscherm.h"

void kernel_main(BootInfo *bootinfo)
{

    //
    // beelscherm initialiseren
    beeldscherm_laad(bootinfo->graphics_info);
    while (1)
    {
        // Kernel loop
    }
}