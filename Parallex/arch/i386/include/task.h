/*************************************************************************
	> File Name: task.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 17时49分02秒
 ************************************************************************/

#ifndef _TASK_H
#define _TASK_H

#include"vmm.h"
#include"pmm.h"
#include"types.h"
#include"syscall.h"
#include"sync.h"

#define CLONE_VM  0x00000100    /*共享虚拟地址空间*/
#define CLONE_THREAD   0x00000200   /*线程组*/

//typedef uint32  pid_t ;  /*进程描述符*/
#define TASK_NAME_MAX  30   /*进程最大的名称描述符*/

typedef enum task_state{
    
        TASK_UNINIT = 0,       /*未初始化*/
        TASK_SLEEPOING = 1,    /*睡眠*/
        TASK_RUNNABLE = 2,     /*可运行也可能在队列中*/
        TASK_ZOMBIE  = 3       /*僵死*/

}task_state;

/*保存上下文*/
typedef struct context{
        uint32 eip;         /*代码段执行位置*/
        uint32 esp;         /*指向堆栈中即将执行的地址*/
        uint32 ebx;         /*进程数据段选择子*/
        uint32 ecx;         /*一般为循环计数器*/
        uint32 edx;         /*一般为数据段*/
        uint32 esi;         /*源目标偏移*/
        uint32 edi;         /*目的目标偏移*/
        uint32 ebp;         /*内存堆栈指针*/   
}context;


#define MAX_TASK  (4096)
#define MAX_PID   (MAX_TASK * 2)

/*线程内存地址页目录*/
struct mm_struct {
    pgd_t * pgd_dir;

};


/*线程控制块PCB*/
struct task_struct {

        task_state state;
        void *stack;
        pid_t pid;
        char name[TASK_NAME_MAX + 1];
        uint32 runs_time;
        volatile uint32 need_resched;
        struct task_struct *parent;
        struct mm_struct *mm;
        struct pt_regs_t *pt_regs;
        struct context context;
        uint32 flags;
        uint32 exit_code;
        
        struct list_head list;
};

extern  struct list_head task_list;

#define le_to_task(le)   list_entry(le,struct task_struct,list)

/*idle 任务指针*/
extern struct task_struct *glb_idle_task;

/*init 任务指针*/
extern struct task_struct *glb_init_task;

#define task_to_stack(task) ((void *)((uint32)task + STACK_SIZE - 8))

#define current  get_current()

/*获取当前任务*/
struct task_struct *get_current(void);

void task_init(void);

void task_run(struct task_struct *task);

int kernel_thread(int (*func)(void *),void *args,uint32 clone_flags);

struct task_struct *find_task(pid_t pid);

void set_proc_name(struct task_struct *task,char *name);

void cpu_idle(void);

pid_t do_fork(uint32 clone_flags,struct pt_regs_t *pt_regs);

void do_exit(int errno);


/*全局的PID 变量*/
extern pid_t new_pid;

/*线程退出函数*/
void kthread_exit();

#endif
