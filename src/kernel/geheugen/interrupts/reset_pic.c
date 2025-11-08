#include "geheugen.h"

void reset_pic(){
    uint8_t oldpic1 = inportb(PIC1_DATA);
    uint8_t oldpic2 = inportb(PIC2_DATA);
    if((oldpic1==0xFF&&oldpic2==0xFF))
    {
        printk("waarschuwing: PIC is uitgeschakeld\n");
    }
    if(check_apic())
    {
        printk("APIC is mogelijk\n");
        enable_lapic();
    }
    else
    {
        printk("APIC niet mogelijk. STOP\n");
        for(;;);
    }
}