#include "pci.h"

uint8_t get_pci_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    uint32_t data = get_pci_dword(bus,slot,function,offset&0xFC);
    return (data >> ((offset & 3) * 8)) & 0xFF;
}