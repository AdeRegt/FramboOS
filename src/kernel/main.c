#include "bootloader.h"
#include "beeldscherm.h"
#include "pci.h"
#include "bestandensysteem.h"
#include "lua_kernel.h"

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

    lua_State *L = lua_kernel_init();
    if (L == NULL) {
        printk("ERROR: Failed to initialize Lua!\n");
    } else {
        printk("Lua initialized successfully\n");
        
        /* Test Lua: execute a simple string */
        const char *test_code = "kprint('Hello from Lua!')";
        if (lua_kernel_dostring(test_code) != 0) {
            printk("Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }for(;;);

    //
    // PCI apparaten initialiseren
    laad_pci();

    //
    // Klaar!
    // printk("Kernel initialisatie compleet!\n");

    //
    // Wachten op beschikbaarheid van FS
    wachten_op_bestandssysteem();
    beeldscherm_leeg();
    printk("Het bestandssysteem is gesignaleerd!\nBestanden: %s \n",directory());
    

    while (1)
    {
        // Kernel loop
    }
}