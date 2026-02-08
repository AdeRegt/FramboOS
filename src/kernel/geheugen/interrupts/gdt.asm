; see https://wiki.osdev.org/GDT_Tutorial

section .text

global initialise_gdt

section .text
initialise_gdt:
   LGDT  [GDT.Pointer]
; Reload CS register:
   PUSH 0x08                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
   LEA RAX, [rel .reload_CS] ; Load address of .reload_CS into RAX
   PUSH RAX                  ; Push this value to the stack
   RETFQ                     ; Perform a far return, RETFQ or LRETQ depending on syntax
.reload_CS:
   ; Reload data segment registers
   MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET

GDT:
.Null:
    dq 0

.Code:                      ; selector 0x08
    dw 0x0000               ; limit (ignored)
    dw 0x0000               ; base low
    db 0x00                 ; base mid
    db 10011010b            ; present, ring 0, code, readable
    db 00100000b            ; L=1, D=0, G=0
    db 0x00                 ; base high

.Data:                      ; selector 0x10
    dw 0x0000
    dw 0x0000
    db 0x00
    db 10010010b            ; present, ring 0, data, writable
    db 00000000b            ; MUST be zero in long mode
    db 0x00

.Pointer:
    dw GDT.Pointer - GDT - 1
    dq GDT


%include "geheugen/task/taskswitchstub.asm"