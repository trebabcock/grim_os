#ifndef MEMORY_H
#define MEMORY_H

#include <lib/stdint.h>
#include <lib/multiboot.h>

#define KERNEL_START      0xC0000000
#define KERNEL_MALLOC     0xD0000000
#define REC_PAGE_DIR      (uint32_t*) 0xFFFFF000
#define REC_PAGE_TABLE(i) (uint32_t*) (0xFFC00000 + (i << 12))
#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE   (1 << 1)
#define PAGE_FLAG_OWNER   (1 << 9)
#define MEM_SIZE          0x1000

void memory_init(uint32_t mem_high, uint32_t phys_alloc_start);
void memset(void* dest, int val, uint32_t count);
void kmalloc_init(uint32_t);

#endif