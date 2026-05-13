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
%include "geheugen/syscall/syscallstart.asm"

global jump_usermode
extern uprogstart
extern usermode_eflags
jump_usermode:

	mov rcx, qword [uprogstart] ; to be loaded into RIP
	mov r11, qword [usermode_eflags] ; to be loaded into EFLAGS
	o64 sysret ;use "o64 sysret" if you assemble with NASM