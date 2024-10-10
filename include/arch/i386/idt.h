#ifndef IDT_H
#define IDT_H

#include <lib/stdint.h>

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct interrupt_register {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_code;
    uint32_t eip;
    uint32_t csm;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;   
};

void idt_init();
void irq_install_handler(int irq, void (*handler)(struct interrupt_register* ir));
void irq_uninstall_handler(int irq);

#endif