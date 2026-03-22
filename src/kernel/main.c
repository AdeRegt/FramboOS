#include "bootloader.h"
#include "beeldscherm.h"
#include "pci.h"
#include "bestandensysteem.h"

void kernel_main(BootInfo *bootinfo)
{

    //
    // beelscherm initialiseren
    laad_beeldscherm(bootinfo->graphics_info);

    //
    // geheugen initialiseren
    laad_geheugen(bootinfo);

    //
    // FS leeg maken voor het geval de memoryleegte niet goed ging
    bs_leeg();

    //
    // PCI apparaten initialiseren
    laad_pci();

    //
    // Klaar!
    printk("Kernel initialisatie compleet!\n");

    //
    // Wachten op beschikbaarheid van FS
    wachten_op_bestandssysteem();
    printk("Het bestandssysteem is gesignaleerd!\n");
    // exec("hello.bin","")

    while (1)
    {
        // Kernel loop
    }
}