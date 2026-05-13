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

    // IA32_STAR MSR format for SYSCALL/SYSRET (beta = bits 63:32):
    // Bits 63:48 = User code selector (for SYSRET return to user mode)
    // Bits 47:32 = Kernel code selector (for SYSCALL entry to kernel)
    // Bits 31:0  = Reserved (alpha = 0)
    alpha = 0;  // Lower 32 bits (reserved)
    beta = (GDT_USER_CODE << 16) | GDT_KERNEL_CODE;  // Upper 32 bits
    cpu_set_specific_registers(SYSCALL_REG_STAR, alpha, beta);

    alpha = (uint32_t)((uint64_t)syscallentrypoint);
    beta = (uint32_t)(((uint64_t)syscallentrypoint) >> 32);
    cpu_set_specific_registers(SYSCALL_REG_LSTAR, alpha, beta);

    alpha = 0x2002;
    beta = 0;
    cpu_set_specific_registers(SYSCALL_REG_MASK,alpha,beta);
}