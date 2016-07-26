/*************************************************************************
	> File Name: slob.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月26日 星期二 08时35分44秒
 ************************************************************************/

#ifndef _SLOB_H
#define _SLOB_H

#include <types.h>

void slob_init(void);

void *kmalloc(uint32 size);

void kfree(void *addr);


#endif
