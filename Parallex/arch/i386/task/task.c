/*************************************************************************
	> File Name: task.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月02日 星期二 22时34分41秒
 ************************************************************************/

#include "task.h"
#include "pmm.h"
#include "vmm.h"
#include "string.h"
#include "sched.h"
#include "gdt.h"
#include "sched.h"
#include "slob.h"
#include "debug.h"
#include "vagr.h"

/*全局PID的值*/
pid_t new_pid  = 0;

/*内核创建线程*/

int32 kerenl_thread(int (*fn)(void *),void *arg){
    
    struct task_struct *new_task = (struct task_struct *)kmalloc(STACK_SIZE);

    bzero(new_task,sizeof(struct task_struct));

    new_task->state = TASK_RUNNABLE;
    new_task->stack = current;
    new_task->pid   = new_pid++;
    new_task->mm    = NULL;

    uint32 *stack_top = (uint32 *)((uint32)new_task + STACK_SIZE);

    *(--stack_top) = (uint32)arg;
    *(--stack_top) = (uint32)kthread_exit;
    *(--stack_top) = (uint32)fn;

    new_task->context.esp = (uint32)new_task + STACK_SIZE - sizeof(uint32)*3;

    new_task->context.eflage = 0x200;
    new_task->next = running_pro_head;

    struct task_struct *tail  = running_pro_head;

    while(tail->next != running_pro_head){
        tail = tail->next;
    }

    tail->next = new_task;

    return new_task->pid;


}


void kthread_exit(){
    
    register uint32 val __asm__("eax");

    printk("Thead exited with value %d\n",val);

    while(1);

}
