#include "xhci.h"

void laad_pci()
{
    printk("PCI apparaten laden...\n");
    // printk("Zoeken naar een XHCI apparaat:\n");
    // pci_class* xhci_device = find_pci_device(PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER, PCI_SUBCLASS_USB , PCI_INTERFACE_XHCI);
    // if (xhci_device != NULL)
    // {
    //     laad_xhci(xhci_device);
    // }
    // else
    // {
    //     printk("Geen XHCI apparaat gevonden.\n");
    // }
    for (uint8_t bus = 0; bus < PCI_MAX_BUS; bus++)
    {
        for (uint8_t slot = 0; slot < PCI_MAX_SLOT; slot++)
        {
            for (uint8_t function = 0; function < PCI_MAX_FUNCTION; function++)
            {
                pci_class* pci = check_pci_entry(bus, slot, function);
                if(pci != NULL)
                {
                    if (pci->class_code == PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER && pci->subclass == PCI_SUBCLASS_USB && pci->prog_if == PCI_INTERFACE_XHCI)
                    {
                        // printk("XHCI apparaat gevonden op bus %d, slot %d, functie %d\n", bus, slot, function);
                        laad_xhci(pci);
                    }
                }
            }
        }
    }
}