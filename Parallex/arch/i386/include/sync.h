/*************************************************************************
	> File Name: sync.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月11日 星期四 14时56分38秒
 ************************************************************************/

#ifndef _SYNC_H
#define _SYNC_H

#include "common.h"
#include "types.h"

/*保存标志寄存器*/

static inline uint32 __intr_store(void){
    
    if(read_eflags() && FL_IF){
        disable_intr();
        return true;
    }
    return false;
}

static inline void __intr_restore(uint32 flag){
    
    if(flag){
        enable_intr();
    }


}

#define local_intr_store(x)   do {x =  __intr_store();} while(0)

#define local_intr_restore(x)   __intr_restore(x);




#endif
