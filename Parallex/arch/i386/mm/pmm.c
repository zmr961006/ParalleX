/*************************************************************************
	> File Name: pmm.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月25日 星期一 17时54分04秒
 ************************************************************************/

#include "types.h"
#include "common.h"
#include "pmm.h"
#include "vagr.h"
#include "string.h"
//#include "debug.h"
#include "FF_mm.h"
#include "mboot.h"
#include "string.h"
#include "sync.h"
#include "vmm.h"
/*物理起始与结束地址*/

//#define KERNBASE (0x1000)        /*虚拟地址空间暂存页表，这里临时定义*/
//#define KVPAGE_SIZE  KERNBASE
// 物理内存管理算法
static const struct pmm_manager *pmm_manager = &ff_mm_manager;

// 物理页帧数组指针 (内核结束地址[实地址]+内核基址+内核页表保留地址)
static page_t *phy_pages = (page_t *)((uint32)kernel_end + KERNBASE + KVPAGE_SIZE);

// 物理页帧数组长度
static uint32 phy_pages_count;

// 可用物理内存页起始地址
static uint32 pmm_addr_start;

// 可用物理内存页结束地址
static uint32 pmm_addr_end;

// 获取可用内存的起始和结束地址
static void get_ram_info(e820map_t *e820map);

// 物理内存页初始化
static void phy_pages_init(e820map_t *e820map);

void pmm_init(void){

    //show_kernel_memory_map();   /*展示内核所处物理地址*/

    //show_memory_map();
    e820map_t e820map;
    bzer(&e820map,sizeof(e820map_t));
    get_ram_info(&e820map);
    phy_pages_init(&e820map);
    //printk("phy_pages_count = %d\n",phy_pages_count);
    page_init(phy_pages,phy_pages_count);
}

static void get_ram_info(e820map_t *e820map)
{
        mmap_entry_t *mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
        mmap_entry_t *mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;

        mmap_entry_t *map_entry;
        for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry++) {
                if (map_entry->type == MULTIBOOT_TYPE_RAM && map_entry->base_addr_low == RAM_KERNEL_START) {
                        e820map->map[e820map->count].addr_low = map_entry->base_addr_low;
                        e820map->map[e820map->count].addr_high = map_entry->base_addr_high;
                        e820map->map[e820map->count].length_low = map_entry->length_low;
                        e820map->map[e820map->count].length_high = map_entry->length_high;
                        e820map->map[e820map->count].type = E820_ARM;
                        e820map->count++;
                }
        }
}

static void phy_pages_init(e820map_t *e820map)
{
        uint32 phy_mem_length = 0;
        for (uint32 i = 0; i < e820map->count; ++i){
                if (e820map->map[i].addr_low > ZONE_HIGHMEM_ADDR) {
                      break;
                }
                if (e820map->map[i].addr_low + e820map->map[i].length_low > ZONE_HIGHMEM_ADDR) {
                        phy_mem_length = ZONE_HIGHMEM_ADDR;
                        break;
                }
                phy_mem_length = e820map->map[i].length_low;
        }

        uint32 pages_mem_length = sizeof(page_t) * (phy_mem_length / PMM_PAGE_SIZE);
        bzer(phy_pages, pages_mem_length);

        // 物理内存页管理起始地址
        pmm_addr_start = ((uint32)phy_pages - KERNBASE + pages_mem_length + PMM_PAGE_SIZE) & PMM_PAGE_MASK;
        printk("the mm_count is %d\n",e820map->count);
        for (uint32 i = 0; i < e820map->count; ++i){
                uint32 start_addr = e820map->map[i].addr_low;
                uint32 end_addr = e820map->map[i].addr_low + e820map->map[i].length_low;
                if (start_addr < pmm_addr_start) {
                        start_addr = pmm_addr_start;
                }
                if (end_addr > ZONE_HIGHMEM_ADDR) {
                        end_addr = ZONE_HIGHMEM_ADDR;
                }
                printk("uint32 start-addr %x  end_start %x\n",start_addr,end_addr);
                for (uint32 addr = start_addr; addr < end_addr; addr += PMM_PAGE_SIZE) {
                        phy_pages_count++;
                }
                pmm_addr_end = end_addr;
        }

        /*assert(pmm_addr_start == page_to_addr(&phy_pages[0]),
                        "phy_pages_init error pmm_start != &page[0]");
        assert(pmm_addr_end - PMM_PAGE_SIZE == page_to_addr(&phy_pages[phy_pages_count-1]),
                        "phy_pages_init error pmm_end != &page[n-1]");
        assert(&phy_pages[0] == addr_to_page(page_to_addr(&phy_pages[0])),
                        "phy_pages_init error addr_to_page error");
        assert(&phy_pages[1] == addr_to_page(page_to_addr(&phy_pages[1])),
                        "phy_pages_init error addr_to_page error");*/
}

page_t *addr_to_page(uint32 addr)
{
        assert(pmm_addr_start != 0, "memory not init, addr_to_page cannot use");

        return (phy_pages + ((addr & PMM_PAGE_MASK) - pmm_addr_start) / PMM_PAGE_SIZE);
}

uint32 page_to_addr(page_t *page)
{
        assert(pmm_addr_start != 0, "memory not init, addr_to_page cannot use");
        
        return (pmm_addr_start + (uint32)(page - phy_pages) * PMM_PAGE_SIZE);
}

void page_init(page_t *pages, uint32 n)
{
        pmm_manager->page_init(pages, n);
}

uint32 alloc_pages(uint32 n)
{
        uint32 page;
        uint32 eflag;
        
        local_intr_store(eflag);
        //printk("sssss\n");
        page = pmm_manager->alloc_pages(n);
        //printk("hhhhhhhh\n");
        local_intr_restore(eflag);

        return page;
}

void free_pages(uint32 addr, uint32 n)
{
        uint32 eflag;
        
        local_intr_store(eflag);
        pmm_manager->free_pages(addr, n);
        local_intr_restore(eflag);
}

uint32 free_pages_count(void)
{
        return pmm_manager->free_pages_count();
}


