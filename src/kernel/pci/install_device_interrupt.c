#include "pci.h"

void install_device_interrupt(pci_class* device, void (*handler)())
{
    if(device->interrupt==PCI_NO_INTERRUPT)
    {
        int msi_cap = pci_find_capability(device, PCI_CAP_ID_MSI);
        if(msi_cap)
        {
            // printk("Interrupt using MSI at capability offset %x \n",msi_cap);
            uint16_t control = get_pci_dword(device->bus, device->slot, device->function, msi_cap + 2) & 0xFFFF;
            int is_64 = (control & (1 << 7)) != 0;
            if(control & 1)
            {
                printk("MSI was al ingeschakeld\n");
            }

            // // Set MSI address (for x86, usually 0xFEE00000 | (APIC ID << 12))
            uint32_t msi_addr = 0xFEE00000;
            set_pci_dword(device->bus, device->slot, device->function, msi_cap + 4, msi_addr);

            // // Set MSI data (the vector number)
            if (is_64) {
                set_pci_dword(device->bus, device->slot, device->function, msi_cap + 8, 0x25);
            } else {
                set_pci_word(device->bus, device->slot, device->function, msi_cap + 8, (uint16_t)0x25);
            }

            // // Enable MSI
            control |= 1;
            set_pci_word(device->bus, device->slot, device->function, msi_cap + 2, control);

            printk("MSI enabled for device at bus %d slot %d func %d\n", device->bus, device->slot, device->function);
            idt_set_entry(&idt[IDT_OFFSET + 0x25], handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
            idt_set_entry(&idt[0x25], handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
        }
        else
        {
            printk("Device heeft geen interrupt lijn of MSI ondersteuning!\n");
        }
    }
    else
    {
        printk("Interrupt using IDT on pin %d \n",device->interrupt);
        idt_set_entry(&idt[IDT_OFFSET + device->interrupt], handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
    }
    
}