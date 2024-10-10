#include <arch/i386/gdt.h>
#include <arch/i386/ports.h>
#include <kernel/stdio.h>
#include <kernel/memory.h>

struct gdt_entry gdt[6];
struct gdt_ptr gp;
struct tss_entry tss;

extern void load_gdt(struct gdt_ptr*);
extern void load_tss();

struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    struct gdt_entry entry;

    entry.base_low = (base & 0xFFFF);
    entry.base_mid = (base >> 16) & 0xFF;
    entry.base_high = (base >> 24) & 0xFF;

    entry.limit = (limit & 0xFFFF);
    entry.flags = (limit >> 16) & 0x0F;

    entry.flags |= (granularity & 0xF0);
    entry.access = access;

    return entry;
}

struct gdt_entry create_tss_entry(uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t) &tss;
    uint32_t limit = base + sizeof(tss);

    struct gdt_entry entry = create_gdt_entry(base, limit, 0xE9, 0x00);
    
    memset(&tss, 0, sizeof(tss));

    tss.ss0 = ss0;
    tss.esp0 = esp0;

    tss.cs = 0x08 | 0x3;
    tss.ss = 0x10 | 0x3;
    tss.ds = 0x10 | 0x3;
    tss.es = 0x10 | 0x3;
    tss.fs = 0x10 | 0x3;
    tss.gs = 0x10 | 0x3;

    return entry;
}

void print_gdt() {
    char buffer[64];
    for (int i = 0; i < 6; i++) {
        snprintf(buffer, sizeof(buffer), "GDT[%d]: base=0x%x, limit=0x%x, access=0x%x, gran=0x%x\n",
                 i,
                 gdt[i].base_low | (gdt[i].base_mid << 16) | (gdt[i].base_high << 24),
                 gdt[i].limit | ((gdt[i].flags & 0x0F) << 16),
                 gdt[i].access,
                 gdt[i].flags);
        serial_write_string(buffer);
    }

    snprintf(buffer, sizeof(buffer), "GDT Pointer: limit=0x%x, base=0x%x\n", gp.limit, gp.base);
    serial_write_string(buffer);
}

void gdt_init() {
    init_serial();
    serial_write_string("Initializing GDT...\n");

    gdt[0] = create_gdt_entry(0, 0, 0, 0);
    gdt[1] = create_gdt_entry(0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt[2] = create_gdt_entry(0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt[3] = create_gdt_entry(0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt[4] = create_gdt_entry(0, 0xFFFFFFFF, 0xF2, 0xCF);
    gdt[5] = create_tss_entry(0x10, 0);

    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base = (uint32_t) &gdt;

    print_gdt();

    serial_write_string("Loading GDT...\n");
    load_gdt(&gp);
    serial_write_string("GDT loaded.\n");

    serial_write_string("Loading TSS...\n");
    load_tss();
    serial_write_string("TSS loaded.\n");
}
