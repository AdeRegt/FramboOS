#include "geheugen.h"

uint64_t syscall_rax;
uint64_t syscall_rbx;
uint64_t syscall_rcx;
uint64_t syscall_rdx;
uint64_t syscall_rsi;
uint64_t syscall_rdi;
uint64_t syscall_rsp;
uint64_t syscall_rbp;
uint64_t syscall_r8;
uint64_t syscall_r9;
uint64_t syscall_r10;
uint64_t syscall_r11;
uint64_t syscall_r12;
uint64_t syscall_r13;
uint64_t syscall_r14;
uint64_t syscall_r15;

void syscallprobe(){
    printk("==== SYSCALL ====\nRAX: %d\nRBX: %d\nRCX: %d\nRDX: %d\n=================\n",syscall_rax,syscall_rbx,syscall_rcx,syscall_rdx);
    switch(syscall_rax){
        case 1:
            if(syscall_rdi!=1){
                printk("Trying to print on something different!");for(;;);
            }
            for(uint64_t i = 0 ; i < syscall_rdx; i++){
                printk("%c",((uint8_t*)syscall_rsi)[i]);
            }
            break;
        default:
            printk("Unknown systemcall\n");
            asm("cli\nhlt");        
    }
}