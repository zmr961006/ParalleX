/*************************************************************************
	> File Name: vagr.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 10时38分21秒
 ************************************************************************/

#ifndef _VAGR_H
#define _VAGR_H

#include "types.h"
#include "screen_operate.h"

#define va_list char*

#define va_start(ap,first) \
        (ap = (va_list)&first + sizeof(first))

#define va_arg(ap,next)  \
        (*(next *)((ap += sizeof(next)) - sizeof(next)))

#define va_end(p) \
        (p = (va_list)NULL)

int printk(const int8 *format,...);

#endif
