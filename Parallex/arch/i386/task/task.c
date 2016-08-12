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
#include "slob.h"
#include "debug.h"
#include "vagr.h"
#include "gdt.h"
#include "syscall.h"
#include "types.h"
#include "common.h"
#include "sync.h"


extern uint8 kernel_stack[];   /*开启分页以后的内核栈*/
extern uint32 kernel_satck_top; /*内核栈顶*/


/*全局PID的值*/
pid_t new_pid  = 0;

/*任务链表PCB*/
struct list_head task_list;

/*全局初始进程*/
struct task_struct *glb_idle_task;

/*全局初始化进程*/
struct task_struct *glb_init_task;

int nr_task = 0;

uint32 glb_pid_map[MAX_PID/32];

static pid_t alloc_pid(void){
        
    for(uint32 i = 0;i < MAX_PID/32;++i ){
        if(glb_pid_map[i] == 0xFFFFFFFF){
            continue;
        }
        for(uint32 j = 0;j < 32;++j){
            for(uint32 j = 0;j < 32;++j){
                if(((1U << j) & glb_pid_map[i]) == 0){
                    glb_pid_map[i] |= 1u << j;
                    return (pid_t)(i * 32 + j);
                }
            }
        }
    }
    return -1;

}

static void free_pid(pid_t pid){
    
    if(pid < 0 || pid > MAX_PID){
        return;
    }
    glb_pid_map[pid/32] &= ~(1U << (pid % 32));
}

void task_init(void){
    
    INIT_LIST_HEAD(&task_list);
    struct task_struct *idle_task = (struct task_struct *)kernel_stack; /*获取内核栈，夺取CPU执行流*/
    printk("the idle kernel stack = %x\n",kernel_stack);
    bzer(idle_task,sizeof(struct task_struct)); /*清空栈*/
    
    /*设置初始任务内容*/

    idle_task->state = TASK_RUNNABLE;
    idle_task->stack = (void *)kernel_satck_top;
    printk("the idle stack = kernel_satck_top = %x\n",kernel_satck_top);
    idle_task->pid   = alloc_pid();
    idle_task->need_resched = true;
    set_proc_name(idle_task,"idle");
    
    /*任务总数递增*/
    nr_task++;
    /*添加任务进队列*/
    list_add(&idle_task->list,&task_list);

    glb_idle_task = idle_task;
    /*注册系统调用*/
    register_interrupt_handler(0x80,syscall_handler);
    

}

/*声明创建的内核线程入口*/
extern int kthread_entry(void *arg);

/*创建内核线程，并且初始化内核的中断栈信息*/
int kernel_thread(int (*func)(void *),void *args,uint32 clone_flags)
{

    pt_regs_t pt_regs;
    bzer(&pt_regs,sizeof(pt_regs_t));

    pt_regs.cs = KERNEL_CS;
    pt_regs.ds = KERNEL_DS;
    pt_regs.ss = KERNEL_DS;
    pt_regs.ebx = (uint32)func;
    pt_regs.edx = (uint32)args;
    pt_regs.eip = (uint32)kthread_entry;

    return do_fork(clone_flags | CLONE_VM,&pt_regs);

}

extern void switch_to(struct context *from,struct context *to);

void task_run(struct task_struct *task){
    
    if(task != current){
            struct task_struct *prev = current;
            struct task_struct *next = task;
            uint32 intr_flag = false;
            local_intr_store(intr_flag);
            {
                /*tss_load()更新进程的TSS*/
                if(!task && task->mm && task->mm->pgd_dir){
                    /*加载进程的页表*/
                    printk("before switch_to\n");
                }
                
                //printk("before switch_to\n");
                switch_to(&prev->context,&next->context);
                //printk("after switch_to \n");
            }
            local_intr_restore(intr_flag);
    }


}

struct task_struct *get_current(void){
    
    register uint32 esp __asm__ ("esp");

    return (struct task_struct *)(esp & (~(STACK_SIZE - 1)));

}


struct task_struct *find_task(pid_t pid){

    if(pid > 0 && pid < MAX_PID){
            struct list_head *le;
            list_for_each(le,&task_list){
                struct task_struct *task = le_to_task(le);
                if(task->pid == pid){
                    return task;
                }
            }
    }
    return NULL;
}

/*设置进程名字*/
void set_proc_name(struct task_struct *task,char *name){
    
    bzer(task->name,sizeof(task->name));
    strncpy(task->name,name,TASK_NAME_MAX);

}

/*分配一个进程结构体PCB*/
struct task_struct *alloc_task_struct(void){
    
        //void *addr = (void *)alloc_pages(STACK_SIZE/PAGE_SIZE);
        void *addr = (void *)alloc_pages(1);
        assert(addr != 0,"alloc_task_error");
        
        struct task_struct *task = pa_to_ka(addr);
        //struct task_struct *task = (struct task_struct *)kmalloc(STACK_SIZE);
        bzer(task,sizeof(struct task_struct));
        task->state = TASK_UNINIT;
        task->stack = task_to_stack(task);
        task->pid = -1;

        return task;


}

uint32 copy_mm(uint32 clone_flags,struct task_struct *task){
    
    if(!clone_flags && !task){
        return -1;
    }
    return 0;

}


void forkret_s(struct pt_regs_t *pt_regs);

/*准备线程设置中断栈*/
void copy_thread(struct task_struct *task,struct pt_regs_t *pt_regs){

        task->pt_regs = (struct pt_regs_t *)((uint32)task->stack - sizeof(struct pt_regs_t));
        *(task->pt_regs) = *pt_regs;
        task->pt_regs->eax = 0;
        task->pt_regs->esp = (uint32)task->stack;
        task->pt_regs->eflags |= FL_IF;
        
        task->context.eip = (uint32)forkret_s;
        task->context.esp = (uint32)task->pt_regs;


}

/*创建进程*/
pid_t do_fork(uint32 clone_flags, struct pt_regs_t *pt_regs){
    
    if(nr_task >= MAX_TASK){
        return -E_NO_FREE_PROC;
    }
    /*分配PCB*/
    struct task_struct *task = alloc_task_struct();
    printk("the addr = %x\n",task);
    if(!task){
        return -E_NO_MEM;
    }
    /*复制内存*/
    /*if(copy_mm(clone_flags,task) != 0){
        free_pages((uint32)ka_to_pa(task),STACK_SIZE/PAGE_SIZE);
        return -E_NO_MEM;
    }*/
    /*创建本进程执行线程,设置从中断可以跳转到内核/用户代码段*/
    copy_thread(task,pt_regs);

    uint32 intr_flag = false;

    local_intr_store(intr_flag);
    {
        task->pid = alloc_pid();
        list_add(&task->list,&task_list);
        nr_task++;
    }
    local_intr_restore(intr_flag);
    /*准备执行*/
    wakeup_task(task);

    return task->pid;
}

/*进程退出*/
void do_exit(int errno){
    
    uint32 intr_flag = false;
    local_intr_store(intr_flag);
    {
        current->state = TASK_ZOMBIE;
        current->exit_code = errno;
        current->need_resched = true;
        nr_task--;
        //free_pid(current->pid);
    }
    local_intr_restore(intr_flag);
    cpu_idle();

}

/*主动调度*/
void cpu_idle(void){
    if(current->need_resched){
        schedule();
    }
}

