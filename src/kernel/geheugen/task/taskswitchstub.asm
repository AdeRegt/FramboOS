;
; taskswitchstub.asm - Robuuste per-task context switching met FPU/SSE support
;

section .text
align 16

global taskswitchstub
extern acknowledge_interrupt
extern scheduler_tick
extern get_current_task_context

taskswitchstub:
    ; cli

    ; Sla alle algemene registers op op de stack
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Verkrijg de pointer naar de context van de huidige taak
    call get_current_task_context  ; retourneert pointer in rax

    ; Sla GP registers op in cpu_context_t
    mov qword [rax + 0],   r15
    mov qword [rax + 8],   r14
    mov qword [rax + 16],  r13
    mov qword [rax + 24],  r12
    mov qword [rax + 32],  r11
    mov qword [rax + 40],  r10
    mov qword [rax + 48],  r9
    mov qword [rax + 56],  r8
    mov qword [rax + 64],  rdi
    mov qword [rax + 72],  rsi
    mov qword [rax + 80],  rbp

    mov rbx, [rsp + 104]   ; rbx
    mov qword [rax + 88],  rbx
    mov rbx, [rsp + 88]    ; rdx
    mov qword [rax + 96],  rbx
    mov rbx, [rsp + 96]    ; rcx
    mov qword [rax + 104], rbx
    mov rbx, [rsp + 112]   ; rax
    mov qword [rax + 112], rbx

    ; Interrupt stack frame inspectie (gepusht door CPU):
    ; [rsp + 120] = RIP
    ; [rsp + 128] = CS
    ; [rsp + 136] = RFLAGS
    ; [rsp + 144] = RSP (indien CPL == 3)
    ; [rsp + 152] = SS  (indien CPL == 3)

    mov rbx, [rsp + 120]
    mov qword [rax + 120], rbx ; rip
    mov rbx, [rsp + 128]
    mov qword [rax + 128], rbx ; cs
    mov rbx, [rsp + 136]
    mov qword [rax + 136], rbx ; rflags

    mov rdx, [rsp + 128]
    and rdx, 3
    mov qword [rax + 168], rdx ; cpl

    cmp rdx, 3
    je .save_r3
    
    ; Ring 0 frame
    mov qword [rax + 136], 0
    mov qword [rax + 144], 0
    jmp .save_segs

.save_r3:
    mov rbx, [rsp + 144]
    mov qword [rax + 136], rbx ; rsp
    mov rbx, [rsp + 152]
    mov qword [rax + 144], rbx ; ss

.save_segs:
    ; Sla FPU / SSE state op (fxsave vereist 16-byte alignment)
    lea rdi, [rax + 176]
    fxsave [rdi]

    ; Roep scheduler aan om volgende taak te selecteren en interrupt te acknowledgen
    call scheduler_tick

    ; Verkrijg de context pointer van de NIEUWE taak
    call get_current_task_context  ; retourneert nieuwe pointer in rax

    ; Herstel FPU / SSE state van de nieuwe taak
    lea rsi, [rax + 176]
    fxrstor [rsi]

    ; Herstel GP registers van de nieuwe taak
    mov r15, qword [rax + 0]
    mov r14, qword [rax + 8]
    mov r13, qword [rax + 16]
    mov r12, qword [rax + 24]
    mov r11, qword [rax + 32]
    mov r10, qword [rax + 40]
    mov r9,  qword [rax + 48]
    mov r8,  qword [rax + 56]
    mov rdi, qword [rax + 64]
    mov rsi, qword [rax + 72]
    mov rbp, qword [rax + 80]
    mov rbx, qword [rax + 88]
    mov rdx, qword [rax + 96]
    mov rcx, qword [rax + 104]

    ; Schoon onze 15 gepushte GP registers op van de stack (120 bytes)
    add rsp, 120

    ; Verwijder het oude interrupt frame van de CPU
    pop r10
    pop r10
    pop r10

    ; Bepaal of we naar Ring 3 of Ring 0 gaan
    mov r11, qword [rax + 168]
    and r11, 3
    cmp r11, 3
    je .restore_user_frame

    ; Ring 0 target task: bouw interrupt frame en voer iretq uit
    push qword [rax + 136] ; rflags
    push qword [rax + 128] ; cs
    push qword [rax + 120] ; rip
    mov rax, qword [rax + 112]
    sti
    iretq

.restore_user_frame:
    mov rcx, qword [rax + 136] ; target RSP voor user mode
    test rcx, rcx
    jz .use_default_rsp
    mov rsp, rcx

.use_default_rsp:
    push qword [rax + 144]     ; ss
    push qword [rax + 136]     ; rsp
    push qword [rax + 136]     ; rflags
    push qword [rax + 128]     ; cs
    push qword [rax + 120]     ; rip
    mov rax, qword [rax + 112]
    sti
    iretq