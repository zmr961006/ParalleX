/*************************************************************************
	> File Name: sched.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月02日 星期二 22时50分23秒
 ************************************************************************/

#include"sched.h"
#include"pmm.h"
#include"vmm.h"
#include"slob.h"
#include"task.h"
#include"debug.h"
#include"vagr.h"

struct task_struct * running_pro_head = NULL;

struct task_struct * wait_proc_head = NULL;

struct task_struct * current = NULL;

extern uint32 kernel_satck_top;

void init_sched(){
    
    current = (struct task_struct *)(kernel_satck_top - STACK_SIZE);
    current->state = TASK_RUNNABLE;
    current->pid   = new_pid++;
    current->stack = current;
    current->mm    = NULL;

    current->next  = current;
    running_pro_head = current;

}

void schedule(){
    
    if(current){
        change_task_to(current->next);
    }

}

void change_task_to(struct task_struct *next){
    
    if(current != next){
        struct task_struct *prev = current;
        current = next;
        switch_to(&(prev->context),&(current->context));
    }

}
