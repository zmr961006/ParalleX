/*************************************************************************
	> File Name: debug.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 20时20分04秒
 ************************************************************************/

#ifndef _DEBUG_H
#define _DEBUG_H

#include "types.h"
#include "vagr.h"

extern uint8 kern_init_text_start[];
extern uint8 kern_init_text_end[];
extern uint8 kern_init_data_start[];
extern uint8 kern_init_data_end[];
extern uint8 kern_text_start[];
extern uint8 kern_text_end[];
extern uint8 kern_data_start[];
extern uint8 kern_data_end[];

#define  assert(x,message)          \
        do{                         \
           if(!(x)){                \
                painc(messgae)      \
           }                        \
        }while(0)                   \


void painc(char *s){
    printk("%s\n");
}

void show_memory_map(void);       /*print BIOS memory */

void show_kernel_memory_map(void);  /*show kernel mem*/


#endif
