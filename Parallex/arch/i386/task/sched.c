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
#include"list.h"
#include"sync.h"


//struct task_struct * running_pro_head = NULL;

//struct task_struct * wait_proc_head = NULL;

//struct task_struct * current = NULL;

//extern uint32 kernel_satck_top;

/*void init_sched(){
    
    current = (struct task_struct *)(kernel_satck_top - STACK_SIZE);
    current->state = TASK_RUNNABLE;
    current->pid   = new_pid++;
    current->stack = current;    当前运行任务
    current->mm    = NULL;

    current->next  = current;
    running_pro_head = current;

}*/

/*void schedule(){
    
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

}*/

void clock_callback(pt_regs_t *regs){
 
    //printk("jejejejjejej\n");
    schedule();

}

//extern task_struct *task_list;

void schedule(void){
    
    struct task_struct *task_next = NULL;
    struct list_head *le = NULL,*last = NULL;

    uint32 intr_flag  = false;

    local_intr_store(intr_flag);
    {
        current->need_resched = false;
        le = last = (current == glb_idle_task)? &task_list : &(current->list);
        for(le = le->next;le != last;le = le->next){
            if(le != &task_list){
                task_next = le_to_task(le);
                if(task_next->state == TASK_RUNNABLE){
                    break;
                }
            }
        }
        //printk("hello worldA\n");
        if(!task_next || task_next->state != TASK_RUNNABLE){
            task_next  = glb_idle_task;
        }

        task_next->runs_time++;
        if(task_next != current){
            task_run(task_next);
        }

        //printk("hello worldB\n");
    }
    local_intr_restore(intr_flag);

}

void wakeup_task(struct task_struct *task){
    
    if(task->state != TASK_ZOMBIE){
        task->state = TASK_RUNNABLE;
    }

}
