/*************************************************************************
	> File Name: common.h   存放汇编操作
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 09时58分58秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include"types.h"

#define FL_CF  0x00000001
#define FL_PF  0x00000004
#define FL_AF  0x00000010
#define FL_ZF  0x00000040
#define FL_SF  0x00000080
#define FL_TF  0x00000100
#define FL_IF  0x00000200
#define FL_DF  0x00000400
#define FL_OF  0x00000800

#define FL_IOPL_MASK 0x00003000
#define FL_IOPL_0    0x00000000
#define FL_IOPL_1    0x00001000
#define FL_IOPL_2    0x00002000
#define FL_IOPL_3    0x00003000

#define FL_NT  0x00004000
#define FL_RF  0x00010000
#define FL_VM  0x00020000
#define FL_AC  0x00040000

#define FL_VIF 0x00080000
#define FL_VIP 0x00100000
#define FL_ID  0x00200000


#define __barrier__() (__asm__ __volatile__ ( "" ::: "memory" ))

static inline void outb(uint16 port,uint8 value){               /*write a byte*/
    
        __asm__ __volatile__("outb %1,%0": :"dN"(port),"a"(value));

}

static inline uint8 inb(uint16 port){                           /*read a byte*/
    
    uint8 ret;
    
    __asm__ __volatile__("inb %1,%0": "=a"(ret):"dN"(port));
    
    return ret;
}

static inline uint16 inw(uint16 port){
    
    uint16 ret;
    
    __asm__ __volatile__("inw %1,%0" : "=a"(ret):"dN"(port));
    
    return ret;

}

static inline void enable_intr(void){
    
    __asm__ __volatile__("sti");
}

static inline void disable_intr(void){
    
    __asm__ __volatile__ ("cli":::"memory");
}

static inline void cpu_hlt(void){
    
    __asm__ __volatile__("hlt");
}

static inline uint32 read_eflags(void){
    
    uint32 eflags;

    __asm__ __volatile__("pushfl;popl %0":"=r"(eflags));
    
    return eflags;

}

static inline void write_eflags(uint32 eflags){
    
    __asm__ __volatile__ ("pushl %0;popfl"::"r"(eflags));

}

static inline void switch_pgd(uint32 pd){
    
    __asm__ __volatile__ ("mov %0,%%cr3"::"r"(pd));

}

static inline void tlb_reload_page(uint32 va){
    
    __asm__ __volatile__ ("invlpg (%0)"::"a"(va));
}

static inline void load_esp(uint32 esp){
    
    __asm__ __volatile__("mov %0,%%esp"::"r"(esp));
}

#endif
