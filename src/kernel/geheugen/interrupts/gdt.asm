; see https://wiki.osdev.org/GDT_Tutorial

section .text

global flush_gdt

section .text
flush_gdt:
lgdt [rdi]
    ; In 64-bit kun je CS niet moven. 
    ; Een far jump werkt vaak betrouwbaarder in sommige emulators:
    push 0x08
    lea rax, [rel .next]
    push rax
    retfq           ; "Return Far" - vergelijkbaar met iretq maar simpeler
.next:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax      ; SS moet vaak ook 0x10 zijn
    mov fs, ax
    mov gs, ax
    ret


%include "geheugen/task/taskswitchstub.asm"

section .bss

[global syscallentrypoint]
[extern syscallprobe]
[extern syscall_rax]
[extern syscall_rbx]
[extern syscall_rcx]
[extern syscall_rdx]
[extern syscall_rsi]
[extern syscall_rdi]
[extern syscall_rsp]
[extern syscall_rbp]
[extern syscall_r8]
[extern syscall_r9]
[extern syscall_r10]
[extern syscall_r11]
[extern syscall_r12]
[extern syscall_r13]
[extern syscall_r14]
[extern syscall_r15]

global _setjmp
global _longjmp

section .text

; int _setjmp(jmp_buf env)
; rdi = adres van de buffer (eerste argument in x86_64 ABI)
_setjmp:
    mov [rdi + 0],  rbx
    mov [rdi + 8],  rbp
    mov [rdi + 16], r12
    mov [rdi + 24], r13
    mov [rdi + 32], r14
    mov [rdi + 40], r15
    
    ; Sla de stack pointer op zoals die was vóór de call
    lea rdx, [rsp + 8]
    mov [rdi + 48], rdx
    
    ; Sla het return adres op (staat bovenop de stack)
    mov rdx, [rsp]
    mov [rdi + 56], rdx

    xor eax, eax    ; return 0
    ret

; void _longjmp(jmp_buf env, int val)
; rdi = adres van de buffer
; rsi = de waarde die setjmp moet teruggeven
_longjmp:
    mov rbx, [rdi + 0]
    mov rbp, [rdi + 8]
    mov r12, [rdi + 16]
    mov r13, [rdi + 24]
    mov r14, [rdi + 40]
    mov r15, [rdi + 48]
    mov rsp, [rdi + 48] ; herstel stack pointer
    
    mov rax, rsi        ; zet de return waarde (val)
    test eax, eax
    jnz .skip_zero
    inc eax             ; de standaard zegt: als val 0 is, maak er 1 van
.skip_zero:

    jmp [rdi + 56]      ; spring terug naar het opgeslagen adres
    
syscallentrypoint:
    mov qword [syscall_rax],rax
    mov qword [syscall_rbx],rbx
    mov qword [syscall_rcx],rcx
    mov qword [syscall_rdx],rdx
    mov qword [syscall_rsi],rsi
    mov qword [syscall_rdi],rdi
    mov qword [syscall_rsp],rsp
    mov qword [syscall_rbp],rbp
    mov qword [syscall_r8],r8
    mov qword [syscall_r9],r9
    mov qword [syscall_r10],r10
    mov qword [syscall_r11],r11
    mov qword [syscall_r12],r12
    mov qword [syscall_r13],r13
    mov qword [syscall_r14],r14
    mov qword [syscall_r15],r15
    call syscallprobe
    mov rax, qword [syscall_rax]
    mov rbx, qword [syscall_rbx]
    mov rcx, qword [syscall_rcx]
    mov rdx, qword [syscall_rdx]
    mov rsi, qword [syscall_rsi]
    mov rdi, qword [syscall_rdi]
    mov r8, qword [syscall_r8]
    mov r9, qword [syscall_r9]
    mov r10, qword [syscall_r10]
    mov r11, qword [syscall_r11]
    mov r12, qword [syscall_r12]
    mov r13, qword [syscall_r13]
    mov r14, qword [syscall_r14]
    mov r15, qword [syscall_r15]
    swapgs
    o64 sysret