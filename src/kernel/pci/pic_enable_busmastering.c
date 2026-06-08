#include "pci.h"

void pci_enable_busmastering(uint8_t bus, uint8_t slot, uint8_t func){
    uint32_t cmd = get_pci_dword(bus, slot, func, 0x04);
    // Controleer of bit 1 (Memory Space) én bit 2 (Bus Master) aan staan
    if ((cmd & 0x06) != 0x06) {
        // Schrijf 0x07 om I/O (bit 0), Memory (bit 1) en Bus Master (bit 2) te forceren
        set_pci_dword(bus, slot, func, 0x04, cmd | 0x07);
    }
}