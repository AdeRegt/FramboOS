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
            printk("Interrupt using MSI at capability offset %x apicid %d at vector %x \n", msi_cap, get_lapic_id(), intlist);
            
            // Lees offset +0 (dword-aligned). 
            // Byte 0: Cap ID, Byte 1: Next Ptr, Byte 2-3: Message Control
            uint32_t cap_header = get_pci_dword(device->bus, device->slot, device->function, msi_cap);
            
            // Isoleer het 16-bit Message Control register
            uint16_t control = (cap_header >> 16) & 0xFFFF;
            int is_64 = (control & (1 << 7)) != 0;
            
            // Set MSI Address Low (voor x86: 0xFEE00000 | (APIC ID << 12))
            uint32_t msi_addr_low = LAPIC_BASE | (get_lapic_id() << 12);
            set_pci_dword(device->bus, device->slot, device->function, msi_cap + 4, msi_addr_low);

            // Set MSI Address High en Data afhankelijk van 64-bit support
            if (is_64) {
                set_pci_dword(device->bus, device->slot, device->function, msi_cap + 8, 0); // Address High = 0
                set_pci_dword(device->bus, device->slot, device->function, msi_cap + 12, intlist); // Data op +12 (0x0C)
            } else {
                // Gebruik dword write voor uitlijning (data is 16-bit, maar dword padding erbovenop is veilig)
                set_pci_dword(device->bus, device->slot, device->function, msi_cap + 8, intlist); 
            }

            // Enable MSI door bit 0 van het control register te zetten
            control |= 1;
            
            // Bewerk het originele header-dword met de nieuwe control-waarde en schrijf terug
            cap_header = (cap_header & 0x0000FFFF) | ((uint32_t)control << 16);
            set_pci_dword(device->bus, device->slot, device->function, msi_cap, cap_header);

            // Registreer in de IDT
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