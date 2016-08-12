/*************************************************************************
	> File Name: sched.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 17时56分54秒
 ************************************************************************/

#ifndef _SCHED_H
#define _SCHED_H

#include "types.h"
#include "task.h"
#include "sync.h"

/*clock 中断回调函数*/
void clock_callback(pt_regs_t *regs);
/*任务调度*/
void schedule(void);
/*唤醒任务*/
void wakeup_task(struct task_struct *task);

/* 可调度链表*/
//extern struct task_struct *running_pro_head;

/*等待进程链表*/
//extern struct task_struct *wait_proc_head;
/*当前运行任务*/
//extern struct task_struct *current;
/*初始化任务调度*/
//void init_sched();
/*任务调度*/
//void schedule();
/*任务切换准备*/
//void change_task_to(struct task_struct *next);
/*任务切换*/
//void switch_to(struct context *prev,struct context *next);



#endif
