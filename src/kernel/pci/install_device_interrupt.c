#include "pci.h"

uint32_t intlist = 0x21;

void install_device_interrupt(pci_class* device, void (*handler)())
{
    // if(device->interrupt==PCI_NO_INTERRUPT)
    {
        int msix_cap = pci_find_capability(device, PCI_CAP_ID_MSIX);
        if(msix_cap)
        {
            printk("Interrupt using MSIX at capability offset %x \n",msix_cap);
            uint32_t msg_addr = get_pci_dword(device->bus, device->slot, device->function, msix_cap + 4);
            uint32_t msg_data = get_pci_dword(device->bus, device->slot, device->function, msix_cap + 8);
            // printk("MSIX old addr %x data %x \n",msg_addr,msg_data);
        }
        int msi_cap = pci_find_capability(device, PCI_CAP_ID_MSI);
        if(msi_cap)
        {
            printk("Interrupt using MSI at capability offset %x apicid %d at vector %x  \n",msi_cap,get_lapic_id(),intlist);
            uint16_t control = get_pci_dword(device->bus, device->slot, device->function, msi_cap + 2) & 0xFFFF;
            int is_64 = (control & (1 << 7)) != 0;
            
            // Set MSI address (for x86, usually 0xFEE00000 | (APIC ID << 12))
            uint32_t msi_addr = LAPIC_BASE | (get_lapic_id()<<12);
            set_pci_dword(device->bus, device->slot, device->function, msi_cap + 4 , msi_addr);//+4?

            // Set MSI data (the vector number)
            if (is_64) {
                set_pci_dword(device->bus, device->slot, device->function, msi_cap + 8, intlist);
            } else {
                set_pci_word(device->bus, device->slot, device->function, msi_cap + 8, (uint16_t)intlist);
            }

            // // Enable MSI
            control |= 1;
            set_pci_word(device->bus, device->slot, device->function, msi_cap + 2, control);

            // printk("MSI enabled for device at bus %d slot %d func %d\n", device->bus, device->slot, device->function);
            idt_set_entry(&idt[intlist], handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);

            intlist++;
        }
        else
        {
            printk("Interrupt using IDT on pin %d \n",device->interrupt);
            idt_set_entry(&idt[IDT_OFFSET + device->interrupt], handler, GDT_KERNEL_CODE, 0, IDT_TYPE_INTERRUPT_GATE);
        }
    }
    
}