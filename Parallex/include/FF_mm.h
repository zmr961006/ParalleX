/*************************************************************************
	> File Name: FF_mm.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月26日 星期二 08时33分42秒
 ************************************************************************/

#ifndef _FF_MM_H
#define _FF_MM_H

#include"types.h"

void ff_page_init(page_t *pages, uint32 n);
uint32 ff_alloc_pages(uint32 n);
void ff_free_pages(uint32 addr, uint32 n);
uint32 ff_free_pages_count(void);
void ff_show_memory_info(void)  __attribute__((unused));
void ff_show_management_info(void)  __attribute__ ((unused));  
void ff_test_mm(void) __attribute__((unused));

/*管理结构
struct pmm_manager ff_mm_manager = {
                "First_Fit_Memory_Managentment",
                &ff_page_init,
                &ff_alloc_pages,
                &ff_free_pages,
                &ff_free_pages_count
};
struct ff_mm_struct ff_mm_info;
// 内存管理结构
struct ff_mm_struct {
        struct list_head free_list;    // 空闲内存块链
        uint32 mm_addr_start;        // 物理内存页管理起始地址
        uint32 mm_addr_end;          // 物理内存页管理结束地址
        atomic_t phy_page_count;       // 物理内存页的总数量
        atomic_t phy_page_now_count;   // 物理内存页的当前数量
};
*/

extern struct pmm_manager ff_mm_manager;
extern struct ff_mm_struct ff_mm_info;


#endif
