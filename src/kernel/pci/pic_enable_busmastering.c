#include "pci.h"

void pci_enable_busmastering(uint8_t bus, uint8_t slot, uint8_t func){
  uint32_t cmd = get_pci_dword(bus, slot, func, 0x04);
	if (!(cmd & (1 << 2))) {
		// Bus mastering is not enabled, enable it
		set_pci_dword(bus, slot, func, 0x04, cmd | (1 << 2));
	}
}