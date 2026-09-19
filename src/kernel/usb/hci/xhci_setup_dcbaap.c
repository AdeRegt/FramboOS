#include "xhci.h"

void xhci_setup_dcbaap(XHCIControllerSession *session)
{
    session->device_context_base_address_array = (uint64_t*) kalloc();
    define_linear_memory_block((void*)(uintptr_t)(session->device_context_base_address_array),1);
    
    //
    // Initialiseer de Device Context Base Address Array met nullen
    //
    for (int i = 0; i <= XHCI_MAX_SLOTS; i++)
    {
        session->device_context_base_address_array[i] = 0;
    }

    //
    // Als er scratchpad buffers zijn, stel deze dan in
    //
    if (HCSPARAMS2_SPR)
    {
        uint8_t scratchpad_buffer_count = HCSPARAMS2_Scratchpad_Bufs_Hi << 5;
        uint64_t* scratchpad_buffers = kalloc();
        define_linear_memory_block((void*)(uintptr_t)(scratchpad_buffers),1);
        for (uint8_t i = 0; i < scratchpad_buffer_count; i++)
        {
            scratchpad_buffers[i] = (uint64_t) kalloc();
            define_linear_memory_block((void*)(uintptr_t)(scratchpad_buffers[i]),1);
        }
        session->device_context_base_address_array[0] = (uint64_t)(uintptr_t)scratchpad_buffers;
        printd("xhci_setup_dcbaap: Scratchpad buffers ingesteld met %d buffers.\n", scratchpad_buffer_count);
    }

    //
    // Stel de Device Context Base Address Array Pointer in
    //
    DCBAAP_L = (uint64_t)(uintptr_t)session->device_context_base_address_array;
    DCBAAP_H = 0;
    printd("xhci_setup_dcbaap: DCBAAP ingesteld op %x\n", DCBAAP_L);
}