/*************************************************************************
	> File Name: atomic.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 16时36分38秒
 ************************************************************************/

#ifndef _ATOMIC_H
#define _ATOMIC_H

#include "types.h"
#include "vagr.h"
#include "debug.h"

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

static inline uint32 test_bit(int32 nr,volatile void * addr){
    
        int32 oldbit;
        printk("test = %x\n",addr);
        __asm__ __volatile__ ("btl %2,%1; sbbl %0,%0"
                              :"=r"(oldbit)
                              :"m"(*(volatile long *)addr),"Ir"(nr));

        if(oldbit != 0){
            return 1;
        }else{
            return 0;
        }

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
                              :"+m"(v->counter)
                              :"ir"(i));

}

static inline int32 atomic_sub_and_test(atomic_t *v,int32 i){
    
        unsigned char c;
        __asm__ __volatile__ (LOCK_PREFIX "subl %2,%0;sete %1"   /*sete  相等置位*/
                              :"+m"(v->counter),"=qm"(c)
                              :"ir"(i):"memory");
        return c;

}

static inline void atomic_inc(atomic_t *v){
    
        __asm__ __volatile__ (LOCK_PREFIX "incl %0"
                              :"+m"(v->counter));

}

static inline int32 atomic_inc_and_test(atomic_t *v){
    
        unsigned char c;

        __asm__ __volatile__(LOCK_PREFIX "incl %0;sete %1"
                             :"+m"(v->counter),"=qm"(c)     /*q 选择eax,ebx,ecx,edx 其中之一*/
                             ::"memory");
        return (c != 0);
}

static inline void atomic_dec(atomic_t *v){
    
        __asm__ __volatile__ (LOCK_PREFIX "decl %0"
                              : "+m"(v->counter));

}

static inline int32 atomic_dec_and_test(atomic_t *v){
    
        unsigned char c;

        __asm__ __volatile__ (LOCK_PREFIX "decl %0;sete %1"
                              :"+m"(v->counter),"=qm"(c)
                              ::"memory");
        return (c != 0);
    
}


#endif
