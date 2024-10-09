#include <lib/stdint.h>
#include <kernel/vga.h>
#include <lib/multiboot.h>
#include <kernel/stdio.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>

void kmain() { 
    vga_write_string("\nWelcome to grim_OS");
    idt_init();

    while (1) {
        // Do nothing
    }
}

void kinit(uint32_t magic, struct multiboot_info* mb_info) {
    vga_init();

    if (magic != MULTIBOOT_MAGIC) {
        vga_write_string("Invalid Multiboot magic number\n");
        return;
    }

    vga_write_string("Booted with Multiboot-compliant loader\n");

    char buffer[128];

    if (mb_info->flags & MULTIBOOT_FLAG_MEM) {
        snprintf(buffer, sizeof(buffer), "Lower memory: %u KB\n", mb_info->mem_lower);
        vga_write_string(buffer);
        snprintf(buffer, sizeof(buffer), "Upper memory: %u KB\n", mb_info->mem_upper);
        vga_write_string(buffer);
    } else {
        vga_write_string("Memory information is not available.\n");
    }

    if (mb_info->flags & (1 << 2)) { 
        snprintf(buffer, sizeof(buffer), "Command line: %s\n", (char*)mb_info->cmdline);
        vga_write_string(buffer);
    } else {
        vga_write_string("No command line available.\n");
    }

    if (mb_info->flags & (1 << 3)) { 
        snprintf(buffer, sizeof(buffer), "Modules count: %u\n", mb_info->mods_count);
        vga_write_string(buffer);
        snprintf(buffer, sizeof(buffer), "Modules address: 0x%x\n", mb_info->mods_addr);
        vga_write_string(buffer);
    } else {
        vga_write_string("No modules available.\n");
    }

    if (mb_info->flags & (1 << 6)) { 
        vga_write_string("Memory map:\n");
        struct multiboot_mmap_entry *mmap = (struct multiboot_mmap_entry *)mb_info->mmap_addr;
        while ((uint32_t)mmap < mb_info->mmap_addr + mb_info->mmap_length) {
            snprintf(buffer, sizeof(buffer), "Base: 0x%x%x, Length: 0x%x%x, Type: %u\n",
                     (uint32_t)(mmap->addr >> 32), (uint32_t)mmap->addr,
                     (uint32_t)(mmap->len >> 32), (uint32_t)mmap->len,
                     mmap->type);
            vga_write_string(buffer);
            mmap = (struct multiboot_mmap_entry *)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    } else {
        vga_write_string("No memory map available.\n");
    }

    gdt_init();
}
