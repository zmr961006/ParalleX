/*************************************************************************
	> File Name: mboot.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 17时41分10秒
 ************************************************************************/

#ifndef _MBOOT_H
#define _MBOOT_H

#include"types.h"

typedef struct multiboot_t {
    
        uint32 flags;
        uint32 mem_lower;
        uint32 mem_upper;
        uint32 boot_device;
        uint32 cmdline;
        uint32 mods_count;
        uint32 mods_addr;

        uint32 num;
        uint32 size;
        uint32 addr;
        uint32 shndx;

        uint32 mmap_length;
        uint32 mmap_addr;
        
        uint32 drives_length;
        uint32 drives_addr;
        uint32 config_table;
        uint32 boot_loader_name;
        uint32 apm_table;
        uint32 vbe_control_info;
        uint32 vbe_mode_info;
        uint32 vbe_mode;
        uint32 vbe_interface_seg;
        uint32 vbe_interface_off;
        uint32 vbe_interface_len;

}__attribute__((packed)) multiboot_t ;

typedef struct mmap_entry_t{
    
        uint32 size;
        uint32 base_addr_low;
        uint32 base_addr_high;
        uint32 length_low;
        uint32 length_high;
        uint32 type;

}__attribute__((packed)) mmap_entry_t; 

extern multiboot_t * mboot_ptr_tmp;

extern multiboot_t * glb_mboot_ptr;


#endif
