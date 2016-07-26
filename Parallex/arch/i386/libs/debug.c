/*************************************************************************
	> File Name: debug.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 20时26分27秒
 ************************************************************************/

#include "debug.h"
#include "vagr.h"
#include "types.h"
#include "mboot.h"
#include "pmm.h"

void show_memory_map(void){

    uint32 mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32 mmap_length = glb_mboot_ptr->mmap_length;

    printk("Memory map : \n");
    
    mmap_entry_t * mmap = (mmap_entry_t *)mmap_addr;
    for(mmap = (mmap_entry_t *)mmap_addr;(uint32)mmap < mmap_addr + mmap_length;mmap++){
        printk("base_addr = 0x%x %x ,length = 0x%x %x,type = 0x%x\n",
              (uint32)mmap->base_addr_high,(uint32)mmap->base_addr_low,
              (uint32)mmap->length_high,(uint32)mmap->length_low,
              (uint32)mmap->type);
    }
    printk("\n");

}
//extern uint32 kernel_start[];
//extern uint32 kernel_end[];


void show_kernel_memory_map(void){
    
    printk("kernel in memory start : %x\n",kernel_start);
    printk("kernel in memory end :   %x\n",kernel_end);
    printk("\nkernel in memory used : %d KB\n",((kernel_end - kernel_start)/1024));


}

void painc(char *s){
    printk("%s\n",s);
}

