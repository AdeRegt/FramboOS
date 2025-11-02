#include "pci.h"

void laad_pci()
{
    printk("PCI apparaten laden...\n");
    printk("Zoeken naar een XHCI apparaat:\n");
    pci_class* xhci_device = find_pci_device(PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER, PCI_SUBCLASS_USB , PCI_INTERFACE_XHCI);
    if (xhci_device != NULL)
    {
        printk("XHCI apparaat gevonden! Vendor ID: %x, Device ID: %x\n", xhci_device->vendor_id, xhci_device->device_id);
    }
    else
    {
        printk("Geen XHCI apparaat gevonden.\n");
    }
}