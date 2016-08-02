/*************************************************************************
	> File Name: sched.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 17时56分54秒
 ************************************************************************/

#ifndef _SCHED_H
#define _SCHED_H

#include "task.h"

extern struct task_struct *running_proc_head;

extern struct task_struct *wait_proc_head;

extern struct task_struct *current;

void init_sched();

void schedule();

void change_task_to(struct task_struct *next);

void switch_to(struct context *prev,struct context *next);



#endif
