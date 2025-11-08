#include "pci.h"

pci_class* check_pci_entry(uint8_t bus, uint8_t slot, uint8_t function)
{
    uint32_t data = get_pci_dword(bus, slot, function, 0x00);

    if (data != 0xFFFFFFFF)
    {
        uint16_t vendor_id = data & 0xFFFF;
        uint16_t device_id = (data >> 16) & 0xFFFF;

        uint32_t data_start = get_pci_dword(bus, slot, function, 0x08);
        data = data_start >> 16;
        uint8_t subclass = data & 0xFF;
        uint8_t class_code = (data >> 8) & 0xFF;
        data = data_start & 0xFFFF;
        uint8_t prog_if = (data >> 8) & 0xFF;

        data = get_pci_dword(bus, slot, function, 0x3C);
        uint8_t interrupt = data & 0xFF;

        pci_class *pci = (pci_class *) alloc_page();
        pci->bus = bus;
        pci->slot = slot;
        pci->function = function;
        pci->vendor_id = vendor_id;
        pci->device_id = device_id;
        pci->class_code = class_code;
        pci->subclass = subclass;
        pci->prog_if = prog_if;
        pci->interrupt = interrupt;
        pci->bar0 = get_pci_dword(bus, slot, function, 0x10);
        pci->bar1 = get_pci_dword(bus, slot, function, 0x14);
        pci->bar2 = get_pci_dword(bus, slot, function, 0x18);
        pci->bar3 = get_pci_dword(bus, slot, function, 0x1C);
        pci->bar4 = get_pci_dword(bus, slot, function, 0x20);
        pci->bar5 = get_pci_dword(bus, slot, function, 0x24);
        pci->capabilities = get_pci_dword(bus, slot, function, 0x34) & 0xFF;
        return pci;
    }
    return NULL;
}