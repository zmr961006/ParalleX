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

typedef enum task_state{
    
        TASK_UNINIT = 0,
        TASK_SLEEPOING = 1,
        TASK_RUNNABLE = 2,
        TASK_ZOMBIE  = 3

}task_state;

struct context{
        uint32 esp;
        uint32 ebp;
        uint32 ebx;
        uint32 esi;
        uint32 edi;
        uint32 eflags;
};

struct mm_struct {
    pgd_t * pgd_dir;

};

struct task_struct {
        __volatile__ task_state state;
        uint32 pid;
        void *stack;
        struct mm_struct *mm;
        struct context context;
        struct task_struct *next;
};



#endif
