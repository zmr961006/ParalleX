/*************************************************************************
	> File Name: vmm.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月27日 星期三 14时34分36秒
 ************************************************************************/

#ifndef _VMM_H
#define _VMM_H

#include "common.h"
#include "types.h"
#include "list.h"
#include "intr.h"
/*虚拟地址页大小*/
#define PAGE_SIZE  (0x1000)

/*页掩码，用来对齐*/
#define PAGE_MASK  (0xFFFFF000)

/*内核起始虚拟地址*/
#define KERNBASE   (0xC0000000)

/*内核管理大小*/
#define KMEMSIZE   (0x38000000)

/*内核页表大小*/
#define KVPAGE_SIZE (0x400000)

/*内核管理物理内存的顶端*/
#define KERNTOP    (KERNBASE + KMEMSIZE)

/*内核偏移地址*/
#define PAGE_OFFSET KERNBASE 

/*每个页表可以映射的内存数*/
#define PAGE_MAP_SIZE  (0x400000)

/*映射内核管理内存需要的页数*/
#define PTE_COUNT     (KMEMSIZE/PAGE_MAP_SIZE)

/*获取一个地址的页目录项*/
#define PGD_INDEX(x)  (((x) >> 22) & 0x3FF)

/*获取一个地址的页表项*/
#define PTE_INDEX(x)  (((x) >> 12) & 0x3FF)

/*获取一个地址的页内偏移*/
#define OFFSET_INDEX(x)  ((x) & 0xFFF)

/*P -- 0存在 1 在内存中*/
#define PAGE_PRESENT   (0x1)

/*读写表示*/
#define PAGE_WRITE     (0x2)

/*用户标识*/
#define PAGE_USER      (0x4)

/*页表数*/
#define PGD_SIZE   (PAGE_SIZE/sizeof(pte_t))

/*页表内成员数*/
#define PTE_SIZE  (PAGE_SIZE/sizeof(uint32))

#define VM_READ  (1U << 0)
#define VM_WRITE (1U << 1)
#define VN_EXEC  (1U << 2)

/*物理地址转换成内核虚拟地址*/
static inline void *pa_to_ka(void *pa){   

    return (void *)((uint32)pa + KERNBASE);
}

static inline void *ka_to_pa(void *ka){
    return (void *)((uint32)ka - KERNBASE);
}

extern pgd_t  pgd_kernel[];

void vmm_init(void);

/*使用flag 指出页的权限，把物理地址映射到虚拟地址*/
void map(pgd_t *pgd_now,uint32 va,uint32 pa,uint32 flags);

/*取消虚拟地址的物理映射*/
void unmap(pgd_t *pgd_now,uint32 va);

/*虚拟地址va 映射到物理地址则返回1*/
uint32 get_mapping(pgd_t *pgd_now,uint32 va,uint32 *pa);

/*页错误中断的函数处理*/
void do_page_fault(pt_regs_t *regs);

#endif
