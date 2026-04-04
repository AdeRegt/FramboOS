;
; Dit is de stub voor de taskswitch 
; Inspired by Fred Noras OS: 
; - https://github.com/frednora/gramado/blob/main/kernel/gthouse/arch/x86_64/ints/per/hw1.asm
; - https://github.com/frednora/gramado/blob/main/kernel/gthouse/arch/x86_64/ints/per/hw2.asm
;

section .text
align 16

global taskswitchstub
extern acknowledge_interrupt
extern scheduler_tick

extern _contextRIP
extern _contextCS
extern _contextRFLAGS
extern _contextRSP
extern _contextSS
extern _contextRDX
extern _contextRCX
extern _contextRBX
extern _contextRAX
extern _contextRBP
extern _contextRDI
extern _contextRSI
extern _contextR8
extern _contextR9
extern _contextR10
extern _contextR11
extern _contextR12
extern _contextR13
extern _contextR14
extern _contextR15
extern _contextGS
extern _contextFS
extern _contextES
extern _contextDS
extern _contextCPL

taskswitchstub:
    cli

    ; Stack frame. (all double)
    pop qword [_contextRIP]     ; rip
    pop qword [_contextCS]      ; cs  (R3)
    pop qword [_contextRFLAGS]  ; rflags

    push rax
    mov rax, qword [_contextCS]  ; Get CPL
    and rax, 3                   ; Select 2 bits

    cmp rax, 3
    je .R3Thread 
    cmp rax, 0
    je .R0Thread
    pop rax
    hlt

; Stackframe for ring 0 has only 3 elements.
.R0Thread:
    pop rax
    mov qword [_contextRSP], 0
    mov qword [_contextSS], 0
    jmp .AfterStackFrame

; Stack frame for ring 3 has 5 elements.
.R3Thread:
    pop rax
    pop qword [_contextRSP]     ; rsp
    pop qword [_contextSS]      ; ss
    jmp .AfterStackFrame

.AfterStackFrame:
    mov qword [_contextRDX], rdx 
    mov qword [_contextRCX], rcx 
    mov qword [_contextRBX], rbx 
    mov qword [_contextRAX], rax
     
    mov qword [_contextRBP], rbp
 
    mov qword [_contextRDI], rdi 
    mov qword [_contextRSI], rsi 

    mov qword [_contextR8], r8
    mov qword [_contextR9], r9
    mov qword [_contextR10], r10
    mov qword [_contextR11], r11
    mov qword [_contextR12], r12
    mov qword [_contextR13], r13
    mov qword [_contextR14], r14
    mov qword [_contextR15], r15

; Data segments
    xor rax, rax
; FS/GS bases come from MSRs (IA32_FS_BASE, IA32_GS_BASE), 
; not the selector’s descriptor base. 
; Moving a selector into fs/gs does not set their base; 
; you need wrmsr if you use per-CPU or TLS bases. 
; Saving/restoring just the selectors may not preserve 
; what you expect.
    mov ax, gs
    mov word [_contextGS], ax
    mov ax, fs
    mov word [_contextFS], ax
; DS and ES are largely ignored for data addressing in long mode; 
; loading them is typically redundant.
    mov ax, es
    mov word [_contextES], ax
    mov ax, ds
    mov word [_contextDS], ax

; cpl
; Get the first 2 bits of cs.
; see: x64cont.c
    mov rax, qword [_contextCS]  ;(R3)
    and rax, 3
    mov [_contextCPL], rax
   
    ;mov rdi, rsp
    call scheduler_tick

; 64bit
; This is a 64bit pointer to the pml4 table.

; #bugbug
; CR3 refresh: 
; We flush TLB each tick; only do it when switching address spaces. 
; Otherwise it’s needless overhead.

;    mov RAX, CR3  
;    IODELAY 
;    mov CR3, RAX  

; Wait TLB.
    ;IODELAY 
;
; == Restore context ====================
;

    ; Segments
    xor rax, rax
; DS and ES are largely ignored for data addressing in long mode; 
; loading them is typically redundant.
    mov ax, word [_contextDS]
    mov ds, ax
    mov ax, word [_contextES]
    mov es, ax
; FS/GS bases come from MSRs (IA32_FS_BASE, IA32_GS_BASE), 
; not the selector’s descriptor base. 
; Moving a selector into fs/gs does not set their base; 
; you need wrmsr if you use per-CPU or TLS bases. 
; Saving/restoring just the selectors may not preserve 
; what you expect.
    mov ax, word [_contextFS]
    mov fs, ax
    mov ax, word [_contextGS]
    mov gs, ax

    mov r15, qword [_contextR15]
    mov r14, qword [_contextR14]
    mov r13, qword [_contextR13]
    mov r12, qword [_contextR12]
    mov r11, qword [_contextR11]
    mov r10, qword [_contextR10]
    mov r9,  qword [_contextR9]
    mov r8,  qword [_contextR8]

    mov rsi, qword [_contextRSI] 
    mov rdi, qword [_contextRDI] 
    
    mov rbp, qword [_contextRBP] 
    
    mov rax, qword [_contextRAX] 
    mov rbx, qword [_contextRBX] 
    mov rcx, qword [_contextRCX] 
    mov rdx, qword [_contextRDX] 

; --------------------------------------------
; Stack frame saga

    ;----------------------------------------
    ; Now rebuild the hardware-saved stack frame.
    ; The appropriate frame is chosen based on _contextCPL.
    ;----------------------------------------

    mov rax, qword [_contextCS]   ; Get CPL
    and rax, 3                    ; Select 2 bits

    ; Compare
    cmp rax, 3
    je .restore_user_mode
    cmp rax, 0
    je .restore_kernel_mode
    ; jmp .InvalidThread

; Stack frame. (all double)
; Stackframe for ring 0 has only 3 elements.
.restore_kernel_mode:
    push qword [_contextRFLAGS]  ; rflags
    push qword [_contextCS]      ; cs
    push qword [_contextRIP]     ; rip
    jmp .stackframe_done

; Stack frame. (all double)
; Stack frame for ring 3 has 5 elements.
.restore_user_mode:
    push qword [_contextSS]      ; ss
    push qword [_contextRSP]     ; rsp
    push qword [_contextRFLAGS]  ; rflags
    push qword [_contextCS]      ; cs
    push qword [_contextRIP]     ; rip

.stackframe_done:
    mov rax, qword [_contextRAX]


    iretq