#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <lib/stdint.h>

#define MULTIBOOT_MAGIC 0x2BADB002  
#define MULTIBOOT_FLAG_MEM 0x001    

struct multiboot_info {
    uint32_t flags;          
    uint32_t mem_lower;      
    uint32_t mem_upper;      
    uint32_t boot_device;   
    uint32_t cmdline;       
    uint32_t mods_count;    
    uint32_t mods_addr;      
    uint32_t syms[4];        
    uint32_t mmap_length;   
    uint32_t mmap_addr;      
    uint32_t drives_length;  
    uint32_t drives_addr;    
    uint32_t config_table;   
    uint32_t boot_loader_name;  
    uint32_t apm_table;     
    uint32_t vbe_control_info;  
    uint32_t vbe_mode_info; 
    uint16_t vbe_mode;      
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
};

struct multiboot_mmap_entry {
    uint32_t size;           
    uint64_t addr;           
    uint64_t len;            
    #define MULTIBOOT_MEMORY_AVAILABLE 1
    #define MULTIBOOT_MEMORY_RESERVED  2
    uint32_t type;           
} __attribute__((packed));   

#endif
