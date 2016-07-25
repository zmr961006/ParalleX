/*************************************************************************
	> File Name: atomic.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 16时36分38秒
 ************************************************************************/

#ifndef _ATOMIC_H
#define _ATOMIC_H

#include "types.h"

#define LOCK_PREFIX "lock;"

#define ATOMIC_INIT(i) {(i)}

static inline void set_bit(int32 nr,volatile void *addr){

    __asm__ __volatile__ ("btsl %1,%0"
                          :"=m"(*(volatile long *)addr)
                          :"Ir" (nr));

}


static inline void clear_bit(int32 nr,volatile void *addr){
    
    __asm__ __volatile__ ("btrl %1,%0"
                          :"=m"(*(volatile long *)addr)
                          :"Ir"(nr));

}

static inline void change_bit(int32 nr,volatile void *addr){
    
    __asm__ __volatile__ ("btcl %1,%0"
                          :"=m"(*(volatile long *)addr)
                          :"Ir"(nr));

}

static inline bool test_bit(int32 nr,volatile void * addr){
    
        int32 oldbit;
        __asm__ __volatile__ ("btl %2,%1; sbbl %0,%0"
                              :"=r"(oldbit)
                              :"m"(*(volatile long *)addr),"Ir"(nr));

        return (oldbit != 0);

}

static inline int32 atomic_read(const atomic_t *v){
    
        return v->counter;

}

static inline void atomic_set(atomic_t *v,int32 i){
    
        v->counter = i;

}

static inline void atomic_add(atomic_t *v,int32 i){
 
        __asm__ __volatile__ (LOCK_PREFIX "addl %1,%0"
                              :"+m"(v->counter)
                              :"ir"(i));

}

static inline void atomic_sub(atomic_t *v,int32 i){
    
        __asm__ __volatile__ (LOCK_PREFIX "subl %1,%0"
                              :"+"(v->counter))

}



#endif
