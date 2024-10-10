#include <lib/stdint.h>
#include <arch/i386/timer.h>
#include <arch/i386/idt.h>
#include <kernel/vga.h>
#include <arch/i386/ports.h>


uint32_t ticks;
const uint32_t freq = 100;

void on_irq0(struct interrupt_register *ir) {
    ticks++;
}

void timer_init() {
    ticks = 0;
    irq_install_handler(0, &on_irq0);

    uint32_t divisor = 1193180 / freq;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t) (divisor & 0xFF));
    outb(0x40, (uint8_t) ((divisor >> 8) & 0xFF));
}