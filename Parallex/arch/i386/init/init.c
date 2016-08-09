/*************************************************************************
	> File Name: init/init.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月21日 星期四 16时41分38秒
 ************************************************************************/

#include "types.h"
#include "vagr.h"
#include "gdt.h"
#include "intr.h"
#include "clock.h"
#include "pmm.h"
#include "common.h"
#include "mboot.h"
#include "pmm.h"
#include "vmm.h"
#include "slob.h"
#include "FF_mm.h"
#include "task.h"
#include "sched.h"
#include "kio.h"
#include "device.h"
#include "string.h"

extern multiboot_t * glb_mboot_ptr;

/*开启分页机制以后的内核加载指针*/
multiboot_t * glob_mboot_ptr;

/*开启分页以后的内核栈*/
uint8 kernel_stack[STACK_SIZE]  __attribute__((aligned(STACK_SIZE)));

/*内核栈的栈顶*/
uint32 kernel_satck_top = (uint32)kernel_stack + STACK_SIZE;

/*内核使用的临时页表，页目录*/
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t  *)0x1000;
__attribute__((section(".init.data"))) pte_t *pte_low = (pte_t  *)0x2000;
__attribute__((section(".init.data"))) pte_t *pte_high = (pte_t *)0x3000;

/*映射临时页表*/
__attribute__((section(".init.text"))) void mmap_tmp_page(void);

/*启用分页*/
__attribute__((section(".init.text"))) void enable_paging(void);

int kern_init();
/*内核入口函数*/
__attribute__((section(".init.text"))) void kern_entry(void){
    
    mmap_tmp_page();
    enable_paging();

    __asm__ __volatile__ ("mov %0,%%esp\n\t"
                          "xor %%ebp,%%ebp" ::"r"(kernel_satck_top));
    glob_mboot_ptr = (multiboot_t *)((uint32)glb_mboot_ptr + PAGE_OFFSET);

    kern_init();
    
}

__attribute__((section(".init.text"))) void mmap_tmp_page(void){
    
        pgd_tmp[0] = (uint32)pte_low | PAGE_PRESENT | PAGE_WRITE;
        
        for(uint32 i = 0;i < 4; ++i){
            uint32 pgd_idx = PGD_INDEX(PAGE_OFFSET + PAGE_MAP_SIZE * i);
            pgd_tmp[pgd_idx] = ((uint32)pte_high + PAGE_SIZE *i) | PAGE_PRESENT | PAGE_WRITE;
        }

        for(int i  = 0;i < 1024;i++){
            pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
        }
        
        for(int i  = 0;i < 1024 *4;i++ ){
            pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
        }
    
        __asm__ __volatile__ ("mov %0,%%cr3": :"r"(pgd_tmp));
}

__attribute__((section(".init.text"))) void enable_paging(void){
    
        uint32 cr0;
        __asm__ __volatile__ ("mov %%cr0,%0":"=r"(cr0));
        cr0 |= (1U << 31);
        __asm__ __volatile__("mov %0,%%cr0"::"r"(cr0));

}
int out_of_page(){

    printk("i am in page mode\n");
    return 0;

}

uint32 flag = 0;
int thread(void *arg){
    while(1){
        if(flag == 1){
            printk("B");
            flag = 0;
        }
    }


}
int thread2(void *arg){
    while(1){
        
            printk("C");
       
       
    }


}
uint32 thread_user(){
    while(1){  
        if(flag == 0){
            printk("A");
            flag = 1;
        }
    }
}

int kern_init(){
    //printk("start!\n");
    gdt_init();
    idt_init();
    pmm_init();
    vmm_init();
    slob_init();
    clock_init();
    init_sched();
    //device_init();
    //__asm__ __volatile__ ("sti");
    kernel_thread(thread,NULL);
    kernel_thread(thread2,NULL);
    __asm__ __volatile__ ("sti");
    /*uint8 ch = 0;
    printk("        ***Welcome to XiYouLinux OS!***\n");
    printk("        ***My name is Parallex***\n");
    printk("        ***thanks to Hurlex***\n");
    char order[10];
    uint32 i;
    
    printk("\n\n[XiYouLinux@localhost HelloWorld]$ ");
    while(true){
    
        if((ch = getchar()) != 0){
            order[i] = ch;
            printk("%c",ch);
            i++;
        }
        if(strcmp(order,"ls") == 0){
            printk("\n\n.  ..  /home  /bin  /etc  /libs /user\n");
            memset(order,0,sizeof(order));
        }
    }*/
    thread_user();
    out_of_page();
    __asm__ __volatile__ ("hlt");
    return 0;
}
