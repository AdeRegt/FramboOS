#include "pci.h"

int pci_find_capability(pci_class *device, uint8_t cap_id)
{
    uint8_t pos = get_pci_dword(device->bus, device->slot, device->function, PCI_CAP_PTR) & 0xFF;
    while (pos)
    {
        uint32_t id = get_pci_dword(device->bus, device->slot, device->function, pos);
        int capid = id & 0xFF;
        if (capid == cap_id)
        {
            return pos;
        }
        pos = (id >> 8) & 0xFF;
    }
    return 0;
}