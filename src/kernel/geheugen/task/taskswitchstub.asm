;
; Dit is de stub voor de taskswitch 
;

section .text
align 16

global taskswitchstub
extern acknowledge_interrupt
extern scheduler_tick

taskswitchstub:
    cli

    ; Save general-purpose registers
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsi
    push rdi
    push rbp
    push rdx
    push rcx
    push rbx
    push rax

    ; Save interrupt frame (RIP, CS, RFLAGS)
    mov rax, [rsp + 15*8]   ; RIP
    push rax
    mov rax, [rsp + 16*8]   ; CS
    push rax
    mov rax, [rsp + 17*8]   ; RFLAGS
    push rax

    ; Call scheduler(current_context)
    mov rdi, rsp
    call scheduler_tick

    ; Switch to next task stack/context
    mov rsp, rax

    ; Restore general-purpose registers
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rbp
    pop rdi
    pop rsi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    sti
    iretq