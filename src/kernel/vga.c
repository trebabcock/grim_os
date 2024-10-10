#include <kernel/vga.h>
#include <lib/stdint.h>

uint16_t* const vga_buffer = (uint16_t*) VGA_BUFFER_ADDRESS;
static uint32_t vga_row = 0;
static uint32_t vga_col = 0;
static enum vga_color color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

void vga_init() {
    vga_clear_screen();
}

void vga_write_char(char ch) {
    if (ch == '\n') {
        vga_new_line();
    } else {
        if (vga_col >= VGA_BUFFER_WIDTH) {
            vga_new_line();
        }

        uint32_t index = vga_row * VGA_BUFFER_WIDTH + vga_col;
        vga_buffer[index] = ((uint16_t)color << 8) | (uint16_t)ch;
        vga_col++;
    }
}

void vga_write_string(const char* str) {
    while (*str) {
        vga_write_char(*str);
        str++;
    }
}

void vga_clear_screen() {
    for (uint32_t y = 0; y < VGA_BUFFER_HEIGHT; y++) {
        for (uint32_t x = 0; x < VGA_BUFFER_WIDTH; x++) {
            uint32_t index = y * VGA_BUFFER_WIDTH + x;
            vga_buffer[index] = ((uint16_t)color << 8) | ' ';
        }
    }
    vga_row = 0;
    vga_col = 0;
}

void vga_new_line() {
    if (vga_row >= VGA_BUFFER_HEIGHT - 1) {
        for (uint32_t y = 0; y < VGA_BUFFER_HEIGHT - 1; y++) {
            for (uint32_t x = 0; x < VGA_BUFFER_WIDTH; x++) {
                uint32_t from = (y + 1) * VGA_BUFFER_WIDTH + x;
                uint32_t to = y * VGA_BUFFER_WIDTH + x;
                vga_buffer[to] = vga_buffer[from];
            }
        }
        for (uint32_t x = 0; x < VGA_BUFFER_WIDTH; x++) {
            uint32_t index = (VGA_BUFFER_HEIGHT - 1) * VGA_BUFFER_WIDTH + x;
            vga_buffer[index] = ((uint16_t)color << 8) | ' ';
        }
    } else {
        vga_row++;
    }
    vga_col = 0;
}

void vga_backspace() {
    if (vga_col > 0) {
        vga_col--;
        uint32_t index = vga_row * VGA_BUFFER_WIDTH + vga_col;
        vga_buffer[index] = ((uint16_t)color << 8) | ' ';
    } else {
        if (vga_row > 0) {
            vga_row--;
            vga_col = VGA_BUFFER_WIDTH - 1;
            uint32_t index = vga_row * VGA_BUFFER_WIDTH + vga_col;
            vga_buffer[index] = ((uint16_t)color << 8) | ' ';
        }
    }
}
