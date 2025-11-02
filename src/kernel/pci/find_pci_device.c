#include "pci.h"

pci_class* find_pci_device(uint8_t class, uint8_t subclass, uint8_t prog_if)
{
    for (uint8_t bus = 0; bus < PCI_MAX_BUS; bus++)
    {
        for (uint8_t slot = 0; slot < PCI_MAX_SLOT; slot++)
        {
            for (uint8_t function = 0; function < PCI_MAX_FUNCTION; function++)
            {
                pci_class* pci = check_pci_entry(bus, slot, function);
                if(pci != NULL)
                {
                    if (pci->class_code == class && pci->subclass == subclass && pci->prog_if == prog_if)
                    {
                        return pci;
                    }
                }
            }
        }
    }
    return NULL;
}