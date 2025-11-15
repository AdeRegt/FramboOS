#include "xhci.h"

// XHCI specification:
// https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/extensible-host-controler-interface-usb-xhci.pdf


void laad_xhci(pci_class* xhci_device)
{
    XHCIControllerSession* session = &xhci_session[xhci_session_count++];
    session->pci_device = xhci_device;
    session->base_xhci_address = (void*)(uintptr_t)(xhci_device->bar0 & 0xFFFFFFF0);
    // printk("XHCI is beschikbaar op IRQ %d met BAR0 %x \n", xhci_device->interrupt, session->base_xhci_address);

    //
    // Dit is de xhci interrupt handler installatie
    //
    install_device_interrupt(xhci_device, xhci_interrupt_handler);

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

    task_create("xhci_event_watcher", event_watcher);

    IMAN (0) = 0b11;
	IMOD (0) = 0;
	USBCMD = USBCMD | USBCMD_MASK_RS | USBCMD_MASK_INTE;
    sleep(100);

    // printk("XHCI controller is klaar voor gebruik.\n");
}