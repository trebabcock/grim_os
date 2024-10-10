section .note.GNU-stack noalloc noexec nowrite progbits


section .text
global load_gdt

load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.reload_cs
.reload_cs:
    ret

global load_tss

load_tss:
    mov ax, 0x2B
    ltr ax
    ret