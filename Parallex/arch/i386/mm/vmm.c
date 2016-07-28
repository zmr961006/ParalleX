/*************************************************************************
	> File Name: vmm.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月27日 星期三 17时00分29秒
 ************************************************************************/

#include "common.h"
#include "debug.h"
#include "vagr.h"
#include "vmm.h"
#include "pmm.h"
#include "string.h"
#include "intr.h"

/* 内核页目录*/
pgd_t pgd_kernel[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

/*内核页表起始地址*/
static pte_t *pte_addr = (pte_t *)((uint32)kernel_end + KERNBASE);

void vmm_init(void){
    
    register_interrupt_handler(INT_PAGE_FAULT, &do_page_fault);
    
    pte_t (*pte_kernel)[PTE_SIZE] = (pte_t (*)[PTE_SIZE])pte_addr;

    uint32 pgd_idx = PGD_INDEX(PAGE_OFFSET);

    for(uint32 i = pgd_idx;i < pgd_idx + PTE_COUNT;++i){
        pgd_kernel[i] = ((uint32)ka_to_pa(pte_kernel[i])) | PAGE_PRESENT | PAGE_WRITE;

    }
    
    uint32 *pte_start = (uint32 *)(pte_addr + PTE_SIZE * pgd_idx);
    for(uint32 i = 0;i < PTE_SIZE * PTE_COUNT;++i){
        pte_start[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    switch_pgd((uint32)ka_to_pa(pgd_kernel));
}

void map(pgd_t * pgd_now,uint32 va,uint32 pa,uint32 flags){
    
    uint32 pgd_idx = PGD_INDEX(va);
    uint32 pte_idx = PTE_INDEX(va);

    pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

    if(!pte){
            pte = (pte_t *)alloc_pages(1);
            pgd_now[pgd_idx] = (uint32)pte | PAGE_PRESENT | PAGE_WRITE;
            pte = (pte_t *)pa_to_ka(pte);
    }else{
            pte = (pte_t *)pa_to_ka(pte);
    }

    pte[pte_idx] = (pa & PAGE_MASK) | flags;
    tlb_reload_page(va);

}

void unmap(pgd_t *pgd_now,uint32 va){
    
        uint32 pgd_idx = PGD_INDEX(va);
        uint32 pte_idx = PTE_INDEX(va);

        pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
        
        if(!pte){
            return;
        }

        pte = (pte_t *)(pa_to_ka(pte));

        pte[pte_idx] = 0;
        
        tlb_reload_page(va);

}

uint32 get_mapping(pgd_t *pgd_now,uint32 va,uint32 *pa){
    
        uint32 pgd_idx = PGD_INDEX(va);
        uint32 pte_idx = PTE_INDEX(va);

        pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
        if(!pte){
            return 0;    
        }
        pte = (pte_t *)(pa_to_ka(pte));

        if(pte[pte_idx] != 0 && pa){
            *pa = pte[pte_idx] & PAGE_MASK;
            return 1;
        }
        return 0;



}
