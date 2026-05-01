[BITS 64]

section .text

; int _setjmp(jmp_buf env)
; rdi = pointer naar jmp_buf (het geheugen waar we registers opslaan)
global _setjmp
_setjmp:
    ; Sla de "callee-saved" registers op
    mov [rdi], rbx
    mov [rdi + 8], rbp
    mov [rdi + 16], r12
    mov [rdi + 24], r13
    mov [rdi + 32], r14
    mov [rdi + 40], r15
    
    ; Sla de Stack Pointer (rsp) op zoals hij was voor de call
    lea rdx, [rsp + 8]
    mov [rdi + 48], rdx
    
    ; Sla het Return Address op (waat moeten we straks terugkomen?)
    mov rdx, [rsp]
    mov [rdi + 56], rdx

    ; Return altijd 0 bij de eerste keer aanroepen
    xor eax, eax
    ret

; void longjmp(jmp_buf env, int val)
; rdi = pointer naar jmp_buf
; rsi = de waarde die _setjmp moet teruggeven (meestal 1)
global longjmp
longjmp:
    mov eax, esi        ; Zet de returnwaarde klaar
    test eax, eax       ; Als val 0 is, moeten we volgens de C-standaard 1 teruggeven
    jnz .skip_zero
    inc eax
.skip_zero:

    ; Herstel alle registers uit het geheugen
    mov rbx, [rdi]
    mov rbp, [rdi + 8]
    mov r12, [rdi + 16]
    mov r13, [rdi + 24]
    mov r14, [rdi + 40]
    mov r15, [rdi + 48]
    
    ; Herstel de Stack Pointer
    mov rsp, [rdi + 48]
    
    ; Spring direct terug naar het opgeslagen Return Address
    jmp [rdi + 56]