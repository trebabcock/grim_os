#include <arch/i386/ports.h>

#define PIC1            0x20    
#define PIC2            0xA0    
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 1)
#define PIC_EOI         0x20    

#define ICW1_INIT       0x10    
#define ICW1_ICW4       0x01    
#define ICW4_8086       0x01    

void pic_remap(int offset1, int offset2) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);  
    outb(PIC2_DATA, offset2);  

    outb(PIC1_DATA, 0x4);
    outb(PIC2_DATA, 0x2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);  
    }
    outb(PIC1_COMMAND, PIC_EOI);      
}

void unmask_keyboard_irq() {
    uint8_t mask = inb(PIC1_DATA); 
    mask &= ~(1 << 1);             
    outb(PIC1_DATA, mask);         
}

void mask_timer_irq() {
    uint8_t mask = inb(0x21);  
    mask |= (1 << 0);          
    outb(0x21, mask);          
}

void keyboard_init() {
    outb(0x60, 0xF4);  
}