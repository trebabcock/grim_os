section .note.GNU-stack noalloc noexec nowrite progbits

section .data

gdtr:
    dw 0
    dd 0

section .text
global load_gdt
extern kmain

; load_gdt:
;    mov   ax, [esp + 4]
;    mov   [gdtr], ax
;    mov   eax, [esp + 8]
;    mov   [gdtr + 2], eax
;    lgdt  [gdtr]
;    ret

load_gdt:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    lgdt [eax]

    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.reload_cs
.reload_cs:
    jmp kmain

global load_tss

load_tss:
    mov ax, 0x2b
    ltr ax
    ret