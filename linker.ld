ENTRY(_start)

SECTIONS
{
    . = 0x00100000;       /* Load kernel at 1MB */

    .text : {
        *(.multiboot)     /* Multiboot header */
        *(.text)          /* Code section */
    }

    .data : {
        *(.data)          /* Initialized data */
    }

    .bss : {
        *(.bss)           /* Uninitialized data */
    }
}
