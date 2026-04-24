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
    printk("__HALT__");for(;;);
}