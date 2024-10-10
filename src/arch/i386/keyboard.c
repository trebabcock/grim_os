#include <arch/i386/keyboard.h>
#include <arch/i386/idt.h>
#include <arch/i386/ports.h>
#include <kernel/stdio.h>
#include <lib/stdbool.h>
#include <kernel/vga.h>

bool shift;
bool caps_lock;

char lowercase[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', // 0x00 - 0x0D
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // 0x0E - 0x1C
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 0x1D - 0x29
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', // 0x2A - 0x37
    0, ' ' // 0x38 - 0x39 (Space)
};

// Uppercase characters (when Shift or Caps Lock, but not both, is active)
char uppercase[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', // 0x00 - 0x0D
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', // 0x0E - 0x1C
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', // 0x1D - 0x29
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', // 0x2A - 0x37
    0, ' ' // 0x38 - 0x39 (Space)
};

// Symbol characters (for Shift-specific symbols, not affected by Caps Lock)
char symbols[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', // 0x00 - 0x0D
    '\b', '\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '{', '}', '\n', // 0x0E - 0x1C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ':', '"', '~', // 0x1D - 0x29
    0, '|', 0, 0, 0, 0, 0, 0, 0, '<', '>', '?', 0, '*', // 0x2A - 0x37
    0, ' ' // 0x38 - 0x39 (Space)
};

bool is_alpha(uint8_t scan_code) {
    return (scan_code >= 0x10 && scan_code <= 0x19) ||
           (scan_code >= 0x1E && scan_code <= 0x26) ||
           (scan_code >= 0x2C && scan_code <= 0x32);
}

void keyboard_handler(struct interrupt_register* ir) {
    char scan_code = inb(0x60) & 0x7F;
    char press = inb(0x60) & 0x80;

    switch (scan_code) {
        case 1:
        case 29:
        case 56:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 87:
        case 88:
            break;
        case 42:
            if (press == 0) {
                shift = true;
            } else {
                shift = false;
            }
            break;
        case 58:
            if (!caps_lock && press == 0) {
                caps_lock = true;
            } else if (caps_lock && press == 0) {
                caps_lock = false;
            }
            break;
        case 14:
            vga_backspace();
        default:
            if (press == 0) {
                if (is_alpha(scan_code)) {
                    if ((caps_lock && !shift) || (!caps_lock && shift)) {
                        printf("%c", uppercase[scan_code]);
                    } else {
                        printf("%c", lowercase[scan_code]);
                    }
                } else if (shift && !is_alpha(scan_code)) {
                    printf("%c", symbols[scan_code]);
                } else {
                    printf("%c", lowercase[scan_code]);
                }
            }
    }
}

void keyboard_init() {
    shift = false;
    caps_lock = false;
    irq_install_handler(1, &keyboard_handler);
}