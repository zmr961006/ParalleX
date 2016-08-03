/*************************************************************************
	> File Name: slob.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月26日 星期二 09时24分32秒
 ************************************************************************/

#include "types.h"
#include "slob.h"
#include "list.h"
#include "vagr.h"
#include "debug.h"
#include "vmm.h"
#include "pmm.h"
#include "FF_mm.h"



typedef struct slob_block{
        uint32 allocated : 1;
        uint32 length  :  31;
        struct list_head list;
}slob_block_t;


#define SLOB_USED   1
#define SLOB_FREE   0

#define SLOB_PAGE_COUNT  2

#define SLOB_MIN_PART    0x20


LIST_HEAD(slob_head);

#define le_to_block(le) list_entry(le,slob_block_t,list)

void *__slob__alloc_pages(uint32 size){
    
        uint32 addr = alloc_pages(size);
        
        if(addr == 0){
            return NULL;
        }
    
    return pa_to_ka((void *)addr);

}

void slob_print(void){
    
    struct list_head *le = NULL;
    list_for_each(le,&slob_head){
            slob_block_t *block = le_to_block(le);
            printk("Addr %x  length %x\n",block,block->length);
    }
    printk("\n");
}

void slob_test(void){

    slob_print();
    
    void *addr1 = kmalloc(100);
    void *addr2 = kmalloc(200);
    void *addr3 = kmalloc(300);

    kfree(addr2);
    kfree(addr3);

    void *addr4 = kmalloc(2220);
    void *addr5 = kmalloc(240);
    void *addr6 = kmalloc(2230);
    kfree(addr6);
    kfree(addr1);

    void *addr7 = kmalloc(220);
    void *addr8 = kmalloc(1200);

    kfree(addr4);
    kfree(addr8);
    kfree(addr7);
    kfree(addr5);
    kfree(addr6);

    slob_print();

}

void slob_init(void){

        slob_block_t *block = __slob__alloc_pages(SLOB_PAGE_COUNT);
        assert(block != NULL,"Init_slob error!\n");
        
        block->allocated = SLOB_FREE;
        block->length  = SLOB_PAGE_COUNT * PAGE_SIZE - sizeof(slob_block_t);
        list_add(&block->list,&slob_head);
        //printk("de ma xi ya \n");
        //slob_test();
}

static void split_chunk(slob_block_t *chunk_block,uint32 len){
    
    if(chunk_block->length - len > sizeof(slob_block_t) + SLOB_MIN_PART){
        slob_block_t * new_chunk = (slob_block_t *)((uint32)chunk_block + sizeof(slob_block_t) + len);
        new_chunk->allocated = SLOB_FREE;
        new_chunk->length = chunk_block->length - len - sizeof(slob_block_t);

        list_add(&new_chunk->list,&chunk_block->list);

        chunk_block->length = len;
        chunk_block->allocated = SLOB_USED;
    }

}

static void glue_chunk(slob_block_t *chunk_block){

    struct list_head *le = &chunk_block->list;

    if(le->prev != &slob_head){
            slob_block_t * prev_block = le_to_block(le->prev);
            if(prev_block->allocated == SLOB_FREE){
                prev_block->length += (chunk_block->length + sizeof(slob_block_t));
                list_del(&chunk_block->list);
                chunk_block = prev_block;
            }
    
    }
    if(le->next != &slob_head){
            slob_block_t *next_block = le_to_block(le->next);
            if(next_block->allocated == SLOB_FREE){
                    chunk_block->length += (next_block->length + sizeof(slob_block_t));
                    list_del(&next_block->list);
            }
    }

}

void *kmalloc(uint32 size){
        uint32 len = (size > SLOB_MIN_PART) ? size:SLOB_MIN_PART;
        len += sizeof(slob_block_t);

        if(!list_empty(&slob_head)){
            struct list_head *le = NULL;
            list_for_each(le,&slob_head){
                slob_block_t * block = le_to_block(le);
                if(block->allocated == SLOB_FREE && block->length > len){
                    split_chunk(block,len);
                    void * addr = (void *)((uint32)block + sizeof(slob_block_t));
                    return addr;
                }
            }
        }
        return NULL;

}

void kfree(void *addr){
    
    slob_block_t * block = (slob_block_t *)((uint32)addr - sizeof(slob_block_t));
    if(block->allocated != SLOB_USED){
        return ;
    }
    block->allocated = SLOB_FREE;

    glue_chunk(block);
}
