/*************************************************************************
	> File Name: fault.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月27日 星期三 17时26分08秒
 ************************************************************************/

#include "vmm.h"
#include "pmm.h"
#include "debug.h"
#include "intr.h"
#include "common.h"
#include "vagr.h"


void do_page_fault(pt_regs_t *regs){

    uint32 cr2;
    __asm__ __volatile__("mov %%cr2,%0":"=r"(cr2));

    printk("Page fault va = %x\n",cr2);

    if(!(regs->err_code & 0x1)){
        printk("the page is not in mem\n");
    }

    if(!(regs->err_code & 0x2)){
        printk("write error\n");
    }else{
        printk("read error\n");
    }

    if(regs->err_code & 0x4){
        printk("in user mode\n");
    }else{
        printk("in kernel mode\n");
    }

    if(regs->err_code & 0x8){
        printk("Reserved bits being overwriten\n");
    }
    if(regs->err_code & 0x10){
        printk("the fault occured during an instruction fetch\n");
    }

    while(1){
        cpu_hlt();
    }

}

