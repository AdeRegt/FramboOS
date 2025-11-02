.global context_switch
.type context_switch, @function

// void context_switch(cpu_context_t *old, cpu_context_t *new)
context_switch:
    // Save callee-saved registers to *old
    movq %rdi, %rax            // rdi = old, rsi = new
    movq %r15, 0x00(%rax)
    movq %r14, 0x08(%rax)
    movq %r13, 0x10(%rax)
    movq %r12, 0x18(%rax)
    movq %r11, 0x20(%rax)
    movq %r10, 0x28(%rax)
    movq %r9,  0x30(%rax)
    movq %r8,  0x38(%rax)
    movq %rsi, 0x40(%rax)
    movq %rdi, 0x48(%rax)
    movq %rbp, 0x50(%rax)
    movq %rdx, 0x58(%rax)
    movq %rcx, 0x60(%rax)
    movq %rbx, 0x68(%rax)
    movq %rax, 0x70(%rax)      // Save rax last (was holding old pointer)
    movq (%rsp), %rdx          // Save return address (RIP)
    movq %rdx, 0x78(%rax)
    movq %rsp, 0x80(%rax)
    pushfq
    popq %rdx
    movq %rdx, 0x88(%rax)

    // Restore registers from *new
    movq %rsi, %rax
    movq 0x00(%rax), %r15
    movq 0x08(%rax), %r14
    movq 0x10(%rax), %r13
    movq 0x18(%rax), %r12
    movq 0x20(%rax), %r11
    movq 0x28(%rax), %r10
    movq 0x30(%rax), %r9
    movq 0x38(%rax), %r8
    movq 0x40(%rax), %rsi
    movq 0x48(%rax), %rdi
    movq 0x50(%rax), %rbp
    movq 0x58(%rax), %rdx
    movq 0x60(%rax), %rcx
    movq 0x68(%rax), %rbx
    movq 0x70(%rax), %rax
    movq 0x80(%rax), %rsp
    movq 0x88(%rax), %rdx
    pushq %rdx
    popfq
    movq 0x78(%rax), %rdx
    movq %rdx, (%rsp)          // Set return address (RIP)
    ret