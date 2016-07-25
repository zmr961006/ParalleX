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

int kern_entry(){
    printk("start!\n");
    gdt_init();
    idt_init();
    pmm_init();
    printk("hello world!\n");
    //clock_init();
    //__asm__ __volatile__ ("sti");
    return 0;
}
