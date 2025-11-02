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
    geheugen_initialiseer(bootinfo);

    //
    // Klaar!
    printk("Kernel initialisatie compleet!\n");
    
    while (1)
    {
        // Kernel loop
    }
}