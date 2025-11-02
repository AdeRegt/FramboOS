#include "pci.h"

uint32_t get_pci_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    uint32_t address = (uint32_t)((1 << 31) | (bus << 16) | (slot << 11) | (function << 8) | (offset & 0xFC));
    outportl(PCI_ADDRESS, address);
    return inportl(PCI_DATA);
}