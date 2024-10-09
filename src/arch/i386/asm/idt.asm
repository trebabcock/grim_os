section .data
idtr:
    dw 0
    dd 0

section .text
global load_idt

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

global isr0_stub
global isr1_stub

extern isr_handler

isr0_stub:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0
    call isr_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iret


isr1_stub:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 1            
    call isr_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iret

global irq0_stub
global irq1_stub

extern irq_handler

irq0_stub:
    cli                
    pusha              
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10       
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 32            
    call irq_handler 

    pop gs             
    pop fs
    pop es
    pop ds
    popa               
    sti                
    iret               

irq1_stub:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 33        
    call irq_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iret

section .note.GNU-stack noalloc noexec nowrite progbits
