#ifndef STDIO_H
#define STDIO_H

#include <kernel/stdarg.h>
#include <lib/stddef.h>

int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list args);

#endif
