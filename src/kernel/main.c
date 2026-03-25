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
    // Syscall instructie aanmelden
    enable_syscall();

    //
    // PCI apparaten initialiseren
    laad_pci();

    //
    // Klaar!
    printk("Kernel initialisatie compleet!\n");

    //
    // Wachten op beschikbaarheid van FS
    wachten_op_bestandssysteem();
    beeldscherm_leeg();
    printk("Het bestandssysteem is gesignaleerd!\nBestanden: %s \n",directory());
    uint8_t* g = bestand("FASM.BIN");
    for(int i = 0 ; i < 0x10 ; i++){printk("%c",g[i]);}

    while (1)
    {
        // Kernel loop
    }
}