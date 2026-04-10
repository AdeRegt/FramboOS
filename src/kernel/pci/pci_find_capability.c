#include "pci.h"

int pci_find_capability(pci_class *device, uint8_t cap_id)
{
    uint8_t status = get_pci_byte(device->bus, device->slot, device->function, 0x06);
    if (!(status & (1 << 4))) 
    {
        return 0;
    }

    uint8_t pos = get_pci_byte(device->bus, device->slot, device->function, PCI_CAP_PTR);
    while (pos)
    {
        uint8_t capid = get_pci_byte(device->bus, device->slot, device->function, pos);
        uint8_t next = get_pci_byte(device->bus, device->slot, device->function, pos+1);
        if (capid == cap_id)
        {
            return pos;
        }
        pos = next;
    }
    return 0;
}