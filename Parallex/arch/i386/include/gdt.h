/*************************************************************************
	> File Name: gdt.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 14时54分19秒
 ************************************************************************/

#ifndef _GDT_H
#define _GDT_H

#include "types.h"

#define GDT_LENGTH     6  /*size of gdt*/

#define SEG_NULL      0
#define SEG_KTEXT     1
#define SEG_KDATA     2
#define SEG_UTEXT     3
#define SEG_UDATA     4
#define SEG_TSS       5

#define  GD_KTEXT     ((SEG_KTEXT) << 3)
#define  GD_KDATA     ((SEG_KDATA) << 3)
#define  GD_UTEXT     ((SEG_UTEXT) << 3)
#define  GD_UDATA     ((SEG_UDATA) << 3)
#define  GD_TSS       ((SEG_TSS)   << 3)

#define DPL_KERNEL    (0)            /*DPL of Segment*/
#define DPL_USER      (3)

#define KERNEL_CS     ((GD_KTEXT) | (DPL_KERNEL))        /*user and kernel DPL*/
#define KERNEL_DS     ((GD_KDATA) | (DPL_KERNEL))
#define USER_CS       ((GD_UTEXT) | (DPL_USER ) )
#define USER_DS       ((GD_UDATA) | (DPL_USER ) )

/*typedef struct tss_entry_t{
    
    uint32 ts_link;
    uint32 ts_esp0;
    uint16 ts_ss0;
    uint16 ts_padding1;

    uint32 ts_esp1;
    uint16 ts_ss1;
    uint16 ts_padding2;

    uint32 ts_esp2;
    uint16 ts_ss2;
    uint16 ts_padding3;

    uint32 ts_cr3;
    uint32 ts_eip;
    uint32 ts_eflags;
    uint32 ts_eax;
    uint32 ts_ecx;
    uint32 ts_edx;
    uint32 ts_ebx;
    uint32 ts_esp;
    uint32 ts_ebp;
    uint32 ts_esi;
    uint32 ts_edi;
    
    uint16 ts_es;
    uint16 ts_padding4;
    uint16 ts_cs;
    uint16 ts_padding5;
    uint16 ts_ss;
    uint16 ts_padding6;
    uint16 ts_ds;
    uint16 ts_padding7;
    uint16 ts_fs;
    uint16 ts_padding8;
    uint16 ts_gs;
    uint16 ts_padding9;
    uint16 ts_ldt;
    uint16 ts_padding10;
    uint16 ts_t;
    uint16 ts_iomb;
}__attribute__((packed)) tss_entry_t;
*/
typedef struct gdt_entry_t{     /*全局描述符结构*/
        uint16 limit_low;       /*段界限 15～0*/
        uint16 base_low;        /*段基地址 12～0*/
        uint8  base_middle;     /*段基地址 23～16*/
        uint8  access;          /*存在位，特权级，描述符类型，描述符类别*/
        uint8  granularity;     /*其它标志，段界限 19～16*/
        uint8  base_high;       /*段基地址 31～24*/
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_t {
    uint16 limit;
    uint32 base;
    
}__attribute__((packed)) gdt_ptr_t;
void gdt_init();

extern void gdt_flush();

//extern void tss_flush();



#endif
