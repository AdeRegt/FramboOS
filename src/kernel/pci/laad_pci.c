#include "xhci.h"

void laad_pci()
{
    printk("PCI apparaten laden...\n");
    printk("Zoeken naar een XHCI apparaat:\n");
    pci_class* xhci_device = find_pci_device(PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER, PCI_SUBCLASS_USB , PCI_INTERFACE_XHCI);
    if (xhci_device != NULL)
    {
        laad_xhci(xhci_device);
    }
    else
    {
        printk("Geen XHCI apparaat gevonden.\n");
    }
}