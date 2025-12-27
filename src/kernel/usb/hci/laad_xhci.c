#include "xhci.h"

// XHCI specification:
// https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/extensible-host-controler-interface-usb-xhci.pdf


void laad_xhci(pci_class* xhci_device)
{
    XHCIControllerSession* session = xhci_allocate_new_session();
    session->pci_device = xhci_device;
    session->base_xhci_address = (void*)(uintptr_t)(xhci_device->bar0 & 0xFFFFFFF0);
    // printk("XHCI is beschikbaar op IRQ %d met BAR0 %x \n", xhci_device->interrupt, session->base_xhci_address);

    #ifdef XHCI_CHECK_INTERNALS
        uint32_t segcnt = ERSTSZ(0); // Aantal segmenten - 1
        uint32_t oldad = ERSTBA_L(0);
        printk("Het XHCI EFI event ring systeem bevat %d segmenten en heeft als address %x \n",segcnt,oldad);
        XHCIEventRingSegmentTable* erst = (XHCIEventRingSegmentTable*) (uint64_t) oldad;
        printk("@ segment size: %d op address %x \n",erst->ring_segment_size,erst->ring_segment_base_address_low);
        uint32_t *oppa = (uint32_t*) (uint64_t) erst->ring_segment_base_address_low;
        uint32_t f = 0;
        for(uint32_t i = 0 ; i < erst->ring_segment_size ; i++){
            uint32_t aa = oppa[(i*4)+0];
            uint32_t ab = oppa[(i*4)+1];
            uint32_t ac = oppa[(i*4)+2];
            uint32_t ad = oppa[(i*4)+3];
            // if( (ad & 0x1) == XHCI_CRCS_DEFAULT_CYCLE_STATE ){
                uint8_t event_type = (ad >> 10) & 0x3F;
                if(event_type){
                    printk("|%d",event_type);f++;
                }
            // }
        }
        printk("@%d ",f);
        return;
    #endif 

    #ifdef ENABLE_XHCI_INTERUPTS
    //
    // Dit is de xhci interrupt handler installatie
    //
    install_device_interrupt(xhci_device, xhci_interrupt_handler);
    #endif 

    //
    // Voer BIOS handoff uit
    //
    perform_bios_handoff(session);

    //
    // Schakel bus mastering in
    //
    pci_enable_busmastering(xhci_device->bus, xhci_device->slot, xhci_device->function);

    //
    // Stop controller
    //
    xhci_stop(session);

    //
    // Reset controller
    //
    xhci_reset(session);

    //
    // Wacht tot de controller klaar is
    //
    wait_for_controller_not_ready(session);

    //
    // Maximale poorten instellen 
    //
    xhci_set_max_ports(session);

    //
    // Stel DCBAAP in en scratchpad buffers als die er zijn
    //
    xhci_setup_dcbaap(session);

    //
    // Stel de Command Ring in
    //
    xhci_setup_commandring(session);

    //
    // Stel de Event Ring in
    //
    xhci_setup_eventring(session);

    task_create(XHCI_EVENT_HANDLER_TREAT_NAME, event_watcher);

    IMAN (0) = 0b10;
	// IMOD (0) = 0;
    #ifdef ENABLE_XHCI_INTERUPTS
	USBCMD = USBCMD | USBCMD_MASK_RS | USBCMD_MASK_INTE;
    #else 
	USBCMD = USBCMD | USBCMD_MASK_RS;
    #endif 
    // printk("XHCI controller is klaar voor gebruik.\n");
}