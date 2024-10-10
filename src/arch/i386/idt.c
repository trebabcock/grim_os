#include <arch/i386/idt.h>
#include <arch/i386/pic.h>
#include <arch/i386/ports.h>
#include <kernel/vga.h>

static struct idt_entry idt[256];
static struct idt_ptr ip;

extern void load_idt(struct idt_ptr* idtp);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void isr128();
extern void isr177();

struct idt_entry create_idt_entry(uint32_t handler, uint16_t sel, uint8_t flags) {
    struct idt_entry entry;
    entry.base_low = handler & 0xFFFF;
    entry.sel = sel;
    entry.zero = 0;
    entry.flags = flags;
    entry.base_high = (handler >> 16) & 0xFFFF;
    return entry;
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        idt[i].base_low = 0;
        idt[i].sel = 0;
        idt[i].zero = 0;
        idt[i].flags = 0;
        idt[i].base_high = 0;
    }

    idt[0] = create_idt_entry((uint32_t)isr0, 0x08, 0x8E);
    idt[1] = create_idt_entry((uint32_t)isr1, 0x08, 0x8E);
    idt[2] = create_idt_entry((uint32_t)isr2, 0x08, 0x8E);
    idt[3] = create_idt_entry((uint32_t)isr3, 0x08, 0x8E);
    idt[4] = create_idt_entry((uint32_t)isr4, 0x08, 0x8E);
    idt[5] = create_idt_entry((uint32_t)isr5, 0x08, 0x8E);
    idt[6] = create_idt_entry((uint32_t)isr6, 0x08, 0x8E);
    idt[7] = create_idt_entry((uint32_t)isr7, 0x08, 0x8E);
    idt[8] = create_idt_entry((uint32_t)isr8, 0x08, 0x8E);
    idt[9] = create_idt_entry((uint32_t)isr9, 0x08, 0x8E);
    idt[10] = create_idt_entry((uint32_t)isr10, 0x08, 0x8E);
    idt[11] = create_idt_entry((uint32_t)isr11, 0x08, 0x8E);
    idt[12] = create_idt_entry((uint32_t)isr12, 0x08, 0x8E);
    idt[13] = create_idt_entry((uint32_t)isr13, 0x08, 0x8E);
    idt[14] = create_idt_entry((uint32_t)isr14, 0x08, 0x8E);
    idt[15] = create_idt_entry((uint32_t)isr15, 0x08, 0x8E);
    idt[16] = create_idt_entry((uint32_t)isr16, 0x08, 0x8E);
    idt[17] = create_idt_entry((uint32_t)isr17, 0x08, 0x8E);
    idt[18] = create_idt_entry((uint32_t)isr18, 0x08, 0x8E);
    idt[19] = create_idt_entry((uint32_t)isr19, 0x08, 0x8E);
    idt[20] = create_idt_entry((uint32_t)isr20, 0x08, 0x8E);
    idt[21] = create_idt_entry((uint32_t)isr21, 0x08, 0x8E);
    idt[22] = create_idt_entry((uint32_t)isr22, 0x08, 0x8E);
    idt[23] = create_idt_entry((uint32_t)isr23, 0x08, 0x8E);
    idt[24] = create_idt_entry((uint32_t)isr24, 0x08, 0x8E);
    idt[25] = create_idt_entry((uint32_t)isr25, 0x08, 0x8E);
    idt[26] = create_idt_entry((uint32_t)isr26, 0x08, 0x8E);
    idt[27] = create_idt_entry((uint32_t)isr27, 0x08, 0x8E);
    idt[28] = create_idt_entry((uint32_t)isr28, 0x08, 0x8E);
    idt[29] = create_idt_entry((uint32_t)isr29, 0x08, 0x8E);
    idt[30] = create_idt_entry((uint32_t)isr30, 0x08, 0x8E);
    idt[31] = create_idt_entry((uint32_t)isr31, 0x08, 0x8E);

    idt[32] = create_idt_entry((uint32_t)irq0, 0x08, 0x8E); 
    idt[33] = create_idt_entry((uint32_t)irq1, 0x08, 0x8E); 
    idt[34] = create_idt_entry((uint32_t)irq2, 0x08, 0x8E); 
    idt[35] = create_idt_entry((uint32_t)irq3, 0x08, 0x8E); 
    idt[36] = create_idt_entry((uint32_t)irq4, 0x08, 0x8E); 
    idt[37] = create_idt_entry((uint32_t)irq5, 0x08, 0x8E); 
    idt[38] = create_idt_entry((uint32_t)irq6, 0x08, 0x8E); 
    idt[39] = create_idt_entry((uint32_t)irq7, 0x08, 0x8E); 
    idt[40] = create_idt_entry((uint32_t)irq8, 0x08, 0x8E); 
    idt[41] = create_idt_entry((uint32_t)irq9, 0x08, 0x8E); 
    idt[42] = create_idt_entry((uint32_t)irq10, 0x08, 0x8E);
    idt[43] = create_idt_entry((uint32_t)irq11, 0x08, 0x8E);
    idt[44] = create_idt_entry((uint32_t)irq12, 0x08, 0x8E);
    idt[45] = create_idt_entry((uint32_t)irq13, 0x08, 0x8E);
    idt[46] = create_idt_entry((uint32_t)irq14, 0x08, 0x8E);
    idt[47] = create_idt_entry((uint32_t)irq15, 0x08, 0x8E);


    idt[128] = create_idt_entry((uint32_t)isr128, 0x08, 0x8E | 0x60);
    idt[177] = create_idt_entry((uint32_t)isr177, 0x08, 0x8E | 0x60);

    ip.limit = sizeof(idt) - 1;
    ip.base = (uint32_t) &idt;

    pic_remap(0x20, 0x28);

    load_idt(&ip);

    __asm__ __volatile__ ("sti");
}

const char* interrupt_messages[] = {
    "Division by Zero Exception",           // ISR 0
    "Debug Exception",                      // ISR 1
    "Non-Maskable Interrupt",               // ISR 2
    "Breakpoint Exception",                 // ISR 3
    "Overflow Exception",                   // ISR 4
    "BOUND Range Exceeded Exception",       // ISR 5
    "Invalid Opcode Exception",             // ISR 6
    "Device Not Available Exception",       // ISR 7
    "Double Fault Exception",               // ISR 8 (has error code)
    "Coprocessor Segment Overrun",          // ISR 9
    "Invalid TSS Exception",                // ISR 10 (has error code)
    "Segment Not Present Exception",        // ISR 11 (has error code)
    "Stack-Segment Fault",                  // ISR 12 (has error code)
    "General Protection Fault",             // ISR 13 (has error code)
    "Page Fault",                           // ISR 14 (has error code)
    "Reserved Exception",                   // ISR 15
    "x87 Floating-Point Exception",         // ISR 16
    "Alignment Check Exception",            // ISR 17
    "Machine Check Exception",              // ISR 18
    "SIMD Floating-Point Exception",        // ISR 19
    "Virtualization Exception",             // ISR 20
    "Reserved Exception",                   // ISR 21
    "Reserved Exception",                   // ISR 22
    "Reserved Exception",                   // ISR 23
    "Reserved Exception",                   // ISR 24
    "Reserved Exception",                   // ISR 25
    "Reserved Exception",                   // ISR 26
    "Reserved Exception",                   // ISR 27
    "Reserved Exception",                   // ISR 28
    "Reserved Exception",                   // ISR 29
    "Reserved Exception",                   // ISR 30
    "Reserved Exception",                   // ISR 31
};

void isr_handler(struct interrupt_register* ir) {
    if (ir->int_no < 32) {
        vga_write_string(interrupt_messages[ir->int_no]);
        vga_write_char('\n');
    }
}

void *irq_routines[16]= {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct interrupt_register* ir)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_handler(struct interrupt_register* ir) {
    void (*handler)(struct interrupt_register* ir);

    handler = irq_routines[ir->int_no - 32];

    if (handler) {
        handler(ir);
    }

    if (ir->int_no >= 40) {
        outb(PIC2, PIC1);
    }

    outb(PIC1, PIC1);
}