#include <lib/stdint.h>
#include <kernel/stdio.h>
#include <kernel/vga.h>

void putchar(char c) {
    vga_write_char(c);
}

static void itoa(int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;
    int is_negative = 0;

    if (base == 10 && value < 0) {
        is_negative = 1;
        value = -value;
    }

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    while (value != 0) {
        tmp_value = value % base;
        *ptr++ = (tmp_value < 10) ? (tmp_value + '0') : (tmp_value + 'a' - 10);
        value /= base;
    }

    if (is_negative) {
        *ptr++ = '-';
    }

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

static void uitoa(uint32_t value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    while (value != 0) {
        tmp_value = value % base;
        *ptr++ = (tmp_value < 10) ? (tmp_value + '0') : (tmp_value + 'a' - 10);
        value /= base;
    }
    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

int vprintf(const char *format, va_list args) {
    char buffer[1024];
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    
    for (int i = 0; i < len; i++) {
        putchar(buffer[i]);
    }
    
    return len;
}

int vsnprintf(char *str, size_t size, const char *format, va_list args) {
    size_t i = 0;
    size_t str_index = 0;

    while (format[i] != '\0' && str_index < size - 1) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            if (format[i] == 'd') {
                int num = va_arg(args, int);
                char num_str[32];
                itoa(num, num_str, 10);  
                for (size_t j = 0; num_str[j] != '\0' && str_index < size - 1; j++) {
                    str[str_index++] = num_str[j];
                }
            } else if (format[i] == 'x') {
                uint32_t num = va_arg(args, uint32_t);
                char num_str[32];
                uitoa(num, num_str, 16); 
                for (size_t j = 0; num_str[j] != '\0' && str_index < size - 1; j++) {
                    str[str_index++] = num_str[j];
                }
            } else if (format[i] == 'u') {
                uint32_t num = va_arg(args, uint32_t);
                char num_str[32];
                uitoa(num, num_str, 10); 
                for (size_t j = 0; num_str[j] != '\0' && str_index < size - 1; j++) {
                    str[str_index++] = num_str[j];
                }
            } else if (format[i] == 's') {
                const char *arg_str = va_arg(args, const char *);
                while (*arg_str && str_index < size - 1) {
                    str[str_index++] = *arg_str++;
                }
            } else if (format[i] == 'c') {
                char c = (char) va_arg(args, int);
                if (str_index < size - 1) {
                    str[str_index++] = c;
                }
            }
        } else {
            str[str_index++] = format[i];
        }
        i++;
    }

    str[str_index] = '\0';
    return str_index;
}


int snprintf(char *str, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}
