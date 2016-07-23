/*************************************************************************
	> File Name: gdt.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 15时19分15秒
 ************************************************************************/

#include "gdt.h"
#include "types.h"

//typedef struct gdt_entry_t{     /*全局描述符结构*/
//        uint16 limit_low;       /*段界限 15～0*/
//        uint16 base_low;        /*段基地址 12～0*/
//        uint8  base_middle;     /*段基地址 23～16*/
//        uint8  access;          /*存在位，特权级，描述符类型，描述符类别*/
//        uint8  granularity;     /*其它标志，段界限 19～16*/
//        uint8  base_high;       /*段基地址 31～24*/
//} __attribute__((packed)) gdt_entry_t;

extern uint32 stack;

gdt_entry_t gdt_entries[GDT_LENGTH] __attribute__((aligned(8)));

static void gdt_set_gate(uint32 num,uint32 base,uint32 limit,uint8 access,uint8 gran){
    /*注册一个全局描述符*/
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16)& 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;

}

//static tss_entry_t tss_entry __attribute__ ((aligned(8)));

/*static void tss_set_gate(int32 num,uint16 ss0,uint32 esp0){
    
    uint32 base = (uint32)&tss_entry;              
    uint32 limit = base + sizeof(tss_entry);

    gdt_set_gate(num,base,limit,0x89,0x40);        

    tss_entry.ts_ss0 = ss0;                       
    tss_entry.ts_esp0 = esp0;

    tss_entry.ts_cs = USER_CS;
    tss_entry.ts_ss = USER_DS;
    tss_entry.ts_ds = USER_DS;
    tss_entry.ts_es = USER_DS;
    tss_entry.ts_fs = USER_DS;
    tss_entry.ts_gs = USER_DS;


}*/

/*typedef struct gdt_ptr_t {
    uint16 limit;
    uint32 base;
    
}__attribute__((packed)) gdt_ptr_t;
*/

gdt_ptr_t gdt_ptr;

void gdt_init(){
    
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH -1;
    gdt_ptr.base  = (uint32)&gdt_entries;

    gdt_set_gate(SEG_NULL,0x0,0x0,0x0,0x0);
    gdt_set_gate(SEG_KTEXT,0x0,0xFFFFFFFF,0x9A,0xC0);
    gdt_set_gate(SEG_KDATA,0x0,0xFFFFFFFF,0x92,0xC0);
    gdt_set_gate(SEG_UTEXT,0x0,0xFFFFFFFF,0xFA,0xC0);
    gdt_set_gate(SEG_UDATA,0x0,0xFFFFFFFF,0xF2,0xC0);

    //tss_set_gate(SEG_TSS,KERNEL_DS,0);

    gdt_flush((uint32)&gdt_ptr);
    //tss_flush();

}



