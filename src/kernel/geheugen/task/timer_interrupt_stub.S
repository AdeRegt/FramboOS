.global timer_interrupt_stub

// Interrupt stubs must use the correct calling convention and stack alignment.
// This example assumes the interrupt is delivered with an error code (if not, adjust accordingly).

timer_interrupt_stub:
    // Save general-purpose registers
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    // Save rflags
    pushfq

    // (Optional) Save segment registers if needed

    // Call the C handler (no arguments)
    call timer_interrupt_handler

    // Restore rflags
    popfq

    // Restore general-purpose registers (reverse order)
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

    // Return from interrupt
    iretq