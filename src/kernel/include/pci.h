#include "geheugen.h"

#define PCI_MAX_BUS 255
#define PCI_MAX_SLOT 32
#define PCI_MAX_FUNCTION 8

#define PCI_ADDRESS 0xCF8
#define PCI_DATA 0xCFC

#define PCI_CAP_ID_MSI 0x05
#define PCI_CAP_PTR    0x34

#define PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER 0x0C
#define PCI_SUBCLASS_USB 0x03
#define PCI_INTERFACE_XHCI 0x30

#define PCI_NO_INTERRUPT 0xFF

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t prog_if;
    uint8_t interrupt;
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint8_t capabilities;
} __attribute__((packed)) pci_class;

void laad_pci();
pci_class* check_pci_entry(uint8_t bus, uint8_t slot, uint8_t function);
uint32_t get_pci_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
pci_class* find_pci_device(uint8_t class, uint8_t subclass, uint8_t function);
void install_device_interrupt(pci_class* device, void (*handler)());
int pci_find_capability(pci_class* device, uint8_t cap_id);
void set_pci_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t value);
void set_pci_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint16_t value);
void pci_enable_busmastering(uint8_t bus, uint8_t slot, uint8_t func);