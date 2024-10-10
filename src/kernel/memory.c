#include <kernel/memory.h>
#include <kernel/stdio.h>
#include <lib/multiboot.h>
#include <lib/stdint.h>
#include <lib/math.h>
#include <lib/stdbool.h>

extern uint32_t page_dir[1024];

static uint32_t page_frame_min;
static uint32_t page_frame_max;
static uint32_t total_alloc;

#define NUM_PAGE_DIRS 256
#define NUM_PAGE_FRAMES (0x100000000 / MEM_SIZE / 8)

uint8_t phys_mem_bitmap[NUM_PAGE_FRAMES / 8]; // fix this, bit array?
uint32_t mem_num_vpages;

static uint32_t page_dirs[NUM_PAGE_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t page_dirs_used[NUM_PAGE_DIRS];

static uint32_t heap_start;
static uint32_t heap_size;
static uint32_t threshold;
static bool kmalloc_initialized = false;

void pmm_init(uint32_t mem_low, uint32_t mem_high) {
    page_frame_min = CEIL_DIV(mem_low, MEM_SIZE);
    page_frame_max = mem_high / MEM_SIZE;
    total_alloc = 0;

    memset(phys_mem_bitmap, 0, sizeof(phys_mem_bitmap));
}

void invalidate(uint32_t vaddr) {
    asm volatile("invlpg %0" :: "m"(vaddr));
}

void memory_init(uint32_t mem_high, uint32_t phys_alloc_start) {
    mem_num_vpages = 0;
    page_dir[0] = 0;
    invalidate(0);

    page_dir[1023] = ((uint32_t) page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidate(0xFFFFF000);

    pmm_init(phys_alloc_start, mem_high);
    memset(page_dirs, 0, MEM_SIZE * NUM_PAGE_DIRS);
    memset(page_dirs_used, 0, NUM_PAGE_DIRS);
}

void memset(void* dest, int val, uint32_t count) {
    unsigned char* temp = (unsigned char*) dest;
    for (; count != 0; count--) {
        *temp++ = (unsigned char) val;
    }
}

uint32_t pmm_alloc_page_frame() {
    uint32_t start = page_frame_min / 8 + ((page_frame_min & 7) != 0 ? 1 : 0);
    uint32_t end = page_frame_max / 8 - ((page_frame_max & 7) != 0 ? 1 : 0);

    for (uint32_t b = start; b < end; b++) {
        uint8_t byte = phys_mem_bitmap[b];
        
        if (byte == 0xFF) {
            continue;
        }

        for (uint32_t i = 0; i < 8; i++) {
            bool used = byte >> i & 1;

            if (!used) {
                byte ^= (-1 ^byte) & (1 << i);
                phys_mem_bitmap[b] = byte;
                total_alloc++;

                uint32_t addr = (b * 8 * i) * MEM_SIZE;
                return addr;
            }
        }
    }

    return 0;
}

uint32_t* mem_get_current_page_dir() {
    uint32_t pd;
    asm volatile("mov %%cr3, %0": "=r"(pd));
    pd += KERNEL_START;
    return (uint32_t*) pd;
}

void mem_change_page_dir(uint32_t* pd) {
    pd = (uint32_t*) (((uint32_t) pd) - KERNEL_START);
    asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(pd));
}

void sync_page_dirs() {
    for (int i = 0; i < NUM_PAGE_DIRS; i++) {
        if (page_dirs_used[i]) {
            uint32_t* pd = page_dirs[i];

            for (int i = 768; i < 1023; i++) {
                pd[i] = page_dir[i] & ~PAGE_FLAG_OWNER;
            }
        }
    }
}

void mem_map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    uint32_t *prev_page_dir = 0;

    if (vaddr >= KERNEL_START) {
        prev_page_dir = mem_get_current_page_dir();

        if (prev_page_dir != page_dir) {
            mem_change_page_dir(page_dir);
        }
    }

    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = vaddr >> 12 & 0x3FF;

    uint32_t* pd = REC_PAGE_DIR;
    uint32_t* pt = REC_PAGE_TABLE(pd_index);

    if (!(page_dir[pd_index] & PAGE_FLAG_PRESENT)) {
        uint32_t pt_paddr = pmm_alloc_page_frame();
        page_dir[pd_index] = pt_paddr | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE | PAGE_FLAG_OWNER | flags;
        invalidate(vaddr);

        for (uint32_t i = 0; i < 1024; i++) {
            pt[i] = 0;
        }
    }

    pt[pt_index] = paddr | PAGE_FLAG_PRESENT | flags;
    mem_num_vpages++;
    invalidate(vaddr);

    if (prev_page_dir != 0) {
        sync_page_dirs();
        if (prev_page_dir != page_dir) {
            mem_change_page_dir(prev_page_dir);
        }
    }
}

void change_heap_size(uint32_t new) {
    uint32_t old_page_top = CEIL_DIV(heap_size, MEM_SIZE);
    uint32_t new_page_top = CEIL_DIV(new, MEM_SIZE);

    uint32_t diff = new_page_top - old_page_top;

    for (int i = 0; i < diff; i++) {
        uint32_t phys = pmm_alloc_page_frame();
        mem_map_page(KERNEL_MALLOC + old_page_top * MEM_SIZE + i * MEM_SIZE, phys, PAGE_FLAG_WRITE);
    }
}

void kmalloc_init(uint32_t initialial_heap_size) {
    heap_start = KERNEL_MALLOC;
    heap_size = 0;
    threshold = 0;
    kmalloc_initialized = true;

    change_heap_size(initialial_heap_size);
}