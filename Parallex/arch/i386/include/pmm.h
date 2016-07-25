/*************************************************************************
	> File Name: pmm.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 15时08分48秒
 ************************************************************************/

#ifndef _PMM_H
#define _PMM_H

#include "types.h"
#include "atomic.h"

/*normal stack size*/
#define STACK_SIZE  (0x1000)

/*size of a page*/
#define PMM_PAGE_SIZE (0x1000)

/*mask of page_size ,align 4096*/
#define PMM_PAGE_MASK (0xFFFFF000)

/*kernel start of phy*/
#define PMM_KERNEL_START (0x100000)

/*start and end of kernel*/
extern uint32 kernel_start[];
extern uint32 kernel_end[];

/*kernel after open page*/
extern uint8 kern_stack[STACK_SIZE];

/*top of kernel stack*/
extern uint32 kern_stack_top;

/*e820 bios 自带内存探测功能，使用0X15中断获取机器内存信息*/

#define E820MAX      (20)
#define E820_ARM     (1)
#define E820_ARR     (2)

typedef struct e820map_t {
        uint32 count;
        struct {
            uint32 addr_low;
            uint32 addr_high;
            uint32 length_low;
            uint32 length_high;
            uint32 type;
        }__attribute__((packed)) map[E820MAX];
}e820map_t;

/*内存页类型分为三种，低地址设备内存，普通内存，高地址寻址内存*/
/*type of memory*/
typedef enum mem_zone_t {  
        ZONE_DMA = 0,
        ZONE_NORMAL = 1,
        ZONE_HIGHMEM = 2
}mem_zone_t;


#define ZONE_NORMAL_ADDR (0x1000000)           /*16MB*/
#define ZONE_HIGHMEM_ADDR (0x38000000)         /*896MB*/


/*type of struct page*/
typedef struct page_t {
        atomic_t ref;             /*引用次数*/
        uint32 flag;              /*当前状态*/
        union{
            uint32 ncount;
            uint32 order ;
        };
        struct list_head list;    /*下个页*/
}page_t;

#define PG_RESERVED (0)    /*当前页可用*/
#define PG_NCOUNT   (1)    /*ncount 字段有效*/
#define PG_ORDER    (2)    /*order 字段有效*/

/*set the resserved flag*/
#define set_page_reserved_flag(page)   set_bit(PG_RESERVED,&((page)->flag))

/*clear reserved flag*/
#define clear_page_reserved_flag(page)  clear_bit(PG_RESERVED,&((page)->flag))


/*test page could use */
#define is_page_reserved(page)         test_bit(PG_RESERVED,&((page)->flagh))

#define set_page_ncount_flag(page)     set_bit(PG_NCOUNT,&((page)->flag))

#define clear_page_ncount_flag(page)   clear_bit(PG_NCOUNT,&((page)->flag))

#define is_page_ncount(page)           test_bit(PG_NCOUNT,&((page)->flag))

#define set_page_order_flag(page)      set_bit(PG_ORDER,&((page)->flag))

#define clear_page_order_flag(page)    clear_bit(PG_ORDER,&((page)->flag))

#define is_page_order(page)            test_bit(PG_ORDER,&((page)->flag))


static inline int32 page_ref(page_t *page){
    
        return atomic_read(&page->ref);

}

static inline void set_page_ref(page_t *page,int32 val){
    
        atomic_set(&page->ref,val);

}

static inline void page_ref_inc(page_t * page){
    
        atomic_inc(&page->ref);

}

static inline void page_ref_dec(page_t *page){
    
        atomic_dec(&page->ref);
}

page_t * addr_to_page(uint32 addr);    /*由物理地址算出该页的管理结构位置*/

uint32 page_to_addr(page_t *page);     /*由页的管理结构计算出页所在物理地址*/ 

/*内存管理子系统管理对象，方法*/

struct pmm_manager{                    
    
        const char *name;              /*管理算法*/
        void (*page_init)(page_t *pages,uint32 n);
        uint32 (*alloc_pages)(uint32 n);
        void (*free_pages)(uint32 addr,uint32 n);
        uint32 (*free_pages_count)(void);

};

void pmm_init(void);    /*pmm init*/

void page_init(page_t * pages,uint32 n);   /*内存管理算法初始化*/

uint32 alloc_pages(uint32 n);         

void free_pages(uint32 addr,uint32 n);

#define alloc_page  alloc_pages(1)

#define free_page(addr) free_pages(addr,1)

uint32 free_pages_count(void);            /*当前可用物理页个数*/


void show_memory_map();

#endif
