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


//typedef uint32  pid_t ;  /*进程描述符*/

typedef enum task_state{
    
        TASK_UNINIT = 0,       /*未初始化*/
        TASK_SLEEPOING = 1,    /*睡眠*/
        TASK_RUNNABLE = 2,     /*可运行也可能在队列中*/
        TASK_ZOMBIE  = 3       /*僵死*/

}task_state;

/*保存上下文*/
typedef struct context{
        uint32 esp;         /*指向堆栈中即将执行的地址*/
        uint32 ebp;         /*保存堆栈中函数或者过程的局部变量*/
        uint32 ebx;         /*通常作为内存偏移量*/
        uint32 esi;         /*通常作为内存中复制源地址*/
        uint32 edi;         /*通常作为内存中目的指针来使用*/ 
        uint32 eflags;      /*标志寄存器*/
}context;


/*线程内存地址页目录*/
struct mm_struct {
    pgd_t * pgd_dir;

};

/*线程控制块PCB*/
struct task_struct {
        __volatile__ task_state state;
        pid_t pid;
        void *stack;             /*线程内核栈地址*/
        struct mm_struct *mm;    /*进程内存地址的页目录*/
        struct context context;  /*进程切换需要上下文信息*/
        struct task_struct *next;    /*链表指针*/
};

/*全局的PID 变量*/
extern pid_t new_pid;

/*内核线程的创建*/
int32 kernel_thread(int (*fn)(void*),void *arg);

/*线程退出函数*/
void kthread_exit();

#endif
