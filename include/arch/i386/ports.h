#ifndef PORTS_H
#define PORTS_H

#include <lib/stdint.h>

void init_serial();

int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);
void serial_write_string(const char* str);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);

uint32_t inl(uint16_t port);
void outl(uint16_t port, uint32_t value);

#endif