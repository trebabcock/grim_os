#ifndef PIC_H
#define PIC_H

#include <lib/stdint.h>

void pic_remap(int offset1, int offset2);
void pic_send_eoi(uint8_t irq);
void unmask_keyboard_irq();
void mask_timer_irq();
void keyboard_init();

#endif