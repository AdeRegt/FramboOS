#include "geheugen.h"

void enable_syscall(){
    printk("Enabeling systemcall\n");
    uint32_t alpha;
    uint32_t beta;
    cpu_get_specific_registers(IA32_EFER,&alpha,&beta);
    if(!(alpha & 1)){
        alpha |= 1;
        cpu_set_specific_registers(IA32_EFER,alpha,beta);
    }

    alpha = 0;
    beta = GDT_KERNEL_CODE | ( GDT_KERNEL_CODE << 16 );
    cpu_set_specific_registers(SYSCALL_REG_STAR,alpha,beta);

    alpha = (uint32_t)((uint64_t)syscallentrypoint);
    beta = 0;
    cpu_set_specific_registers(SYSCALL_REG_LSTAR,alpha,beta);

    alpha = 0x2002;
    beta = 0;
    cpu_set_specific_registers(0xC0000084,alpha,beta);
}