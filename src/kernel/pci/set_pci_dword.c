#include "pci.h"

void set_pci_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value)
{
    uint32_t address;
    address = (uint32_t)((bus << 16) | (slot << 11) | (function << 8) | (offset & 0xFC) | 0x80000000);
    outportl(PCI_ADDRESS, address);
    outportl(PCI_DATA, value);
}