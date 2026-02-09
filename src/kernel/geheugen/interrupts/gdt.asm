; see https://wiki.osdev.org/GDT_Tutorial

section .text

global  initialise_gdt

section .text
initialise_gdt:

    ; Set TSS.rsp0
    lea rax,       [KernelStackTop]
    mov [TSS + 4], rax              ; rsp0 is at offset 4

    ; Disable I/O bitmap
    mov word [TSS + 102], 104 ; sizeof(TSS)
   LGDT [GDT.Pointer]
; Reload CS register:
   PUSH  0x08                  ; Push code segment to stack, 0x08 is a stand-in for your code segment
   LEA   RAX, [rel .reload_CS] ; Load address of .reload_CS into RAX
   PUSH  RAX                   ; Push this value to the stack
   RETFQ                       ; Perform a far return, RETFQ or LRETQ depending on syntax
.reload_CS:
   ; Reload data segment registers
   MOV AX, 0x10 ; 0x10 is a stand-in for your data segment
   MOV DS, AX
   MOV ES, AX
   MOV FS, AX
   MOV GS, AX
   MOV SS, AX

    mov ax, 0x18 ; TSS selector
    ltr ax
   RET

GDT:
.Null:
    dq 0

.Code: ; selector 0x08
    dw 0x0000    ; limit (ignored)
    dw 0x0000    ; base low
    db 0x00      ; base mid
    db 10011010b ; present, ring 0, code, readable
    db 00100000b ; L=1, D=0, G=0
    db 0x00      ; base high

.Data: ; selector 0x10
    dw 0x0000
    dw 0x0000
    db 0x00
    db 10010010b ; present, ring 0, data, writable
    db 00000000b ; MUST be zero in long mode
    db 0x00
TSS_BASE  equ TSS
TSS_LIMIT equ TSS_end - TSS - 1
.TSS:
        dw TSS_LIMIT ; limit
    dw 0                        ; base low (ignored in long mode)
    db 0                        ; base mid1
    db 10001001b                ; type=9, present
    db (TSS_LIMIT >> 16) & 0x0F ; granularity
    db 0                        ; base mid2
    dd 0                        ; base high
    dd 0                        ; reserved

.Pointer:
    dw GDT.Pointer - GDT - 1
    dq GDT


%include "geheugen/task/taskswitchstub.asm"

section .bss
align   16
TSS:
    resd 1 ; reserved
    resq 1 ; rsp0
    resq 1 ; rsp1
    resq 1 ; rsp2
    resq 1 ; reserved
    resq 7 ; ist1..ist7
    resq 1 ; reserved
    resw 1 ; reserved
    resw 1 ; iopb_offset
TSS_end:

KernelStack:
    resb 4096
KernelStackTop: