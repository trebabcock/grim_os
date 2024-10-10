#include <lib/stdint.h>
#include <kernel/vga.h>
#include <lib/multiboot.h>
#include <kernel/stdio.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/timer.h>
#include <arch/i386/keyboard.h>
#include <kernel/memory.h>

void kmain(uint32_t magic, struct multiboot_info* boot_info) {
    vga_init();

    if (magic != MULTIBOOT_MAGIC) {
        printf("Invalid Multiboot magic number\n");

        return;
    }

    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();

    uint32_t mod1 = *(uint32_t*) (boot_info->mods_addr + 4);
    uint32_t phys_alloc_start = (mod1 + 0xFFF) & ~0xFFF;

    memory_init(boot_info->mem_upper * 1024, phys_alloc_start);
    kmalloc_init(MEM_SIZE);

    printf("Welcome to grim_OS\n");

    for(;;);
}
