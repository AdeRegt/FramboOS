#include "bootloader.h"
#include "beeldscherm.h"
#include "pci.h"

void kernel_main(BootInfo *bootinfo)
{

    //
    // beelscherm initialiseren
    laad_beeldscherm(bootinfo->graphics_info);

    //
    // geheugen initialiseren
    laad_geheugen(bootinfo);

    //
    // PCI apparaten initialiseren
    laad_pci();

    //
    // Klaar!
    printk("Kernel initialisatie compleet!\n");

    while (1)
    {
        // Kernel loop
    }
}