/*************************************************************************
	> File Name: syscall.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月11日 星期四 11时15分41秒
 ************************************************************************/

#include "types.h"
#include "debug.h"
#include "intr.h"
#include "vagr.h"
#include "syscall.h"

int sys_test(uint32 args[]){

    uint32 errno = (int)args[0];

    printk("\nsys_ run here\n");

    return errno;
}

int sys_exit(uint32 args[]){

    int errno = (int)args[0];

    return errno;

}

int sys_fork(uint32 args[]){
    
    int errno = (int)args[0];

    return errno;

}

int sys_getpid(uint32 args[]){
    
    int errno = (int)args[0];
    
    return errno;
}


int (*syscalls[])(uint32 args[]) = {

    [SYS_test] = sys_test,
    [SYS_exit] = sys_exit,
    [SYS_fork] = sys_fork,
    [SYS_getpid] = sys_getpid

};


void syscall_handler(pt_regs_t *regs){
        
    uint32 args[5];
    int sysno = regs->eax;
    if(sysno >= 0  && sysno <= SYSCALL_MAX){
        if(syscalls[sysno]){
            
            args[0] = regs->ebx;
            args[1] = regs->ecx;
            args[2] = regs->edx;
            args[3] = regs->esi;
            args[4] = regs->edi;

            regs->eax = syscalls[sysno](args);
        }
    }
    return;


}


