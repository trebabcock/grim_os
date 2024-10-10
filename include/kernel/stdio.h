#ifndef STDIO_H
#define STDIO_H

#include <kernel/stdarg.h>
#include <lib/stddef.h>

void putchar(char c);
int printf(const char *format, ...);
int vprintf(const char *format, va_list args);
int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list args);

#endif
