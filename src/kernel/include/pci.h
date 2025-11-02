#include "geheugen.h"

#define PCI_MAX_BUS 255
#define PCI_MAX_SLOT 32
#define PCI_MAX_FUNCTION 8

#define PCI_ADDRESS 0xCF8
#define PCI_DATA 0xCFC

#define PCI_CLASS_CODE_SERIAL_BUS_CONTROLLER 0x0C
#define PCI_SUBCLASS_USB 0x03
#define PCI_INTERFACE_XHCI 0x30

typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t prog_if;
} __attribute__((packed)) pci_class;

void laad_pci();
pci_class* check_pci_entry(uint8_t bus, uint8_t slot, uint8_t function);
uint32_t get_pci_dword(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);
pci_class* find_pci_device(uint8_t class, uint8_t subclass, uint8_t function);