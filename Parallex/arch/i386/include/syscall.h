/*************************************************************************
	> File Name: syscall.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月11日 星期四 10时55分53秒
 ************************************************************************/

#ifndef _SYSCALL_H
#define _SYSCALL_H

#define SYS_test   0
#define SYS_exit   1
#define SYS_fork   2
#define SYS_read   3
#define SYS_write  4
#define SYS_open   5
#define SYS_close  6
#define SYS_getpid 7

#define SYSCALL_MAX 7

void syscall_handler(pt_regs_t *regs);


#endif
