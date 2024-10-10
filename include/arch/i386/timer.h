#ifndef TIMER_H
#define TIMER_H

#include <arch/i386/idt.h>

void timer_init();
void on_irq0(struct interrupt_register* ir);

#endif