#include <arch/i386/ports.h>


#define SERIAL_COM1_BASE 0x3F8          
#define SERIAL_DATA_PORT(base) (base)   
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2) 
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3) 
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)  

void init_serial() {
    outb(SERIAL_LINE_COMMAND_PORT(SERIAL_COM1_BASE), 0x80);    
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), 0x03);            
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE) + 1, 0x00);        
    outb(SERIAL_LINE_COMMAND_PORT(SERIAL_COM1_BASE), 0x03);    
    outb(SERIAL_FIFO_COMMAND_PORT(SERIAL_COM1_BASE), 0xC7);    
    outb(SERIAL_MODEM_COMMAND_PORT(SERIAL_COM1_BASE), 0x0B);   
}

int serial_received() {
    return inb(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 1;
}

char read_serial() {
    while (serial_received() == 0);
    return inb(SERIAL_DATA_PORT(SERIAL_COM1_BASE));
}

int is_transmit_empty() {
    return inb(SERIAL_LINE_STATUS_PORT(SERIAL_COM1_BASE)) & 0x20;
}

void write_serial(char a) {
    while (is_transmit_empty() == 0);
    outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), a);
}

void serial_write_string(const char* str) {
    while (*str) {
        write_serial(*str++);
    }
}

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ __volatile__ (
        "inb %1, %0"
        : "=a"(result) 
        : "Nd"(port)   
    );
    return result;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port) 
    );
}

uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ __volatile__ (
        "inw %1, %0"
        : "=a"(result)
        : "Nd"(port)  
    );
    return result;
}

void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__ (
        "outw %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

uint32_t inl(uint16_t port) {
    uint32_t result;
    __asm__ __volatile__ (
        "inl %1, %0"
        : "=a"(result) 
        : "Nd"(port)   
    );
    return result;
}

void outl(uint16_t port, uint32_t value) {
    __asm__ __volatile__ (
        "outl %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}
