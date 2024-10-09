#include <arch/i386/idt.h>
#include <arch/i386/pic.h>
#include <kernel/vga.h>

static struct idt_entry idt[256];
static struct idt_ptr ip;

extern void load_idt(struct idt_ptr* idtp);

extern void isr0_stub();
extern void isr1_stub();

extern void irq0_stub();
extern void irq1_stub();

struct idt_entry create_idt_entry(uint32_t handler, uint16_t sel, uint8_t flags) {
    struct idt_entry entry;
    entry.base_low = handler & 0xFFFF;
    entry.sel = sel;
    entry.zero = 0;
    entry.flags = flags;
    entry.base_high = (handler >> 16) & 0xFFFF;
    return entry;
}

void isr_handler(uint32_t isr_vec) {
    vga_write_string("Interrupt handled\n");

    switch (isr_vec) {
        case 0:
            vga_write_string("Divide by zero exception\n");
            break;
        case 1:
            vga_write_string("Debug exception\n");
            break;
        default:
            vga_write_string("Unhandled exception\n");
            break;
    }
}

void irq_handler(uint32_t irq_vec) {
    vga_write_string("Interrupt handled\n");

    switch (irq_vec) {
        case 32: 
            vga_write_string("Timer interrupt!\n");
            break;
        case 33: 
            vga_write_string("Keyboard interrupt!\n");
            break;
        default:
            vga_write_string("Unhandled IRQ!\n");
            break;
    }

    pic_send_eoi(irq_vec - 32);
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        idt[i].base_low = 0;
        idt[i].sel = 0;
        idt[i].zero = 0;
        idt[i].flags = 0;
        idt[i].base_high = 0;
    }

    idt[0] = create_idt_entry((uint32_t)isr0_stub, 0x08, 0x8E);
    idt[1] = create_idt_entry((uint32_t)isr1_stub, 0x08, 0x8E);

    idt[32] = create_idt_entry((uint32_t)irq0_stub, 0x08, 0x8E);
    idt[33] = create_idt_entry((uint32_t)irq1_stub, 0x08, 0x8E);

    ip.limit = sizeof(idt) - 1;
    ip.base = (uint32_t) &idt;

    load_idt(&ip);

    pic_remap(0x20, 0x28);
    unmask_keyboard_irq();
    mask_timer_irq();
    // keyboard_init();

    __asm__ __volatile__ ("sti");
}