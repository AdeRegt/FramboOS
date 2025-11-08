#include "pci.h"

void set_pci_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t value)
{
    uint32_t current_dword = get_pci_dword(bus, slot, function, offset & 0xFC);
    uint16_t shift = (offset & 2) * 8;
    uint32_t mask = 0xFFFF << shift;
    uint32_t new_dword = (current_dword & ~mask) | ((uint32_t)value << shift);
    set_pci_dword(bus, slot, function, offset & 0xFC, new_dword);
}