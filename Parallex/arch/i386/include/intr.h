/*************************************************************************
	> File Name: intr.h
	> Author: 中断描述符
	> Mail: 
	> Created Time: 2016年07月23日 星期六 09时20分59秒
 ************************************************************************/

#ifndef _INTR_H
#define _INTR_H

#include "types.h"
#include "vagr.h"

/*寄存器保存类型*/

typedef struct pt_regs_t {
    
        uint16  ds;         /*保护用户的数据段描述符*/
        uint16  padding1;

        uint32  edi;        /*pusha 指令压栈*/
        uint32  esi;
        uint32  ebp;
        uint32  oesp;
        uint32  ebx;
        uint32  edx;
        uint32  ecx;
        uint32  eax;

        uint32  int_no;     /*中断号，内核代码自行压栈*/
        uint32  err_code;   /*错误代码，有中断错误代码中断会由CPU自行压栈*/

        uint32  eip;        /*CPU自动压栈*/ 
        uint16  cs;
        uint16  padding2;
        uint32  eflags;

        uint32  esp;        /*特权级切换CPU压栈*/
        uint16  ss;
        uint16  padding3;

}pt_regs_t;

typedef struct idt_entry_t{
        uint16 base_low;
        uint16 sel ;
        uint8  always0;
        uint8  flags;
        uint16 base_high;
}__attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_t{

        uint16 limit;
        uint32 base;

}__attribute__((packed)) idt_ptr_t;



typedef void (*interrupt_handler_t)(pt_regs_t *);  /*定义中断处理函数指针*/

void isr_handler(pt_regs_t *regs);                 /*调用中断处理函数*/

void register_interrupt_handler(uint8 n,interrupt_handler_t h);  /*注册一个中断处理函数*/

#define  INT_DIVIDE_ERROR     0
#define  INT_DEBUG            1
#define  INT_NMI              2
#define  INT_BREAKPOINT       3
#define  INT_OVERFLOW         4
#define  INT_BOUND            5
#define  INT_INVALID_OPCODE   6
#define  INT_DEVICE_NOT_AVAIL 7
#define  INT_DOUBLE_FAULT     8
#define  INT_COPROCESSOR      9
#define  INT_INVALID_TSS      10
#define  INT_SEGMENT          11
#define  INT_STACK_FAULT      12
#define  INT_GENERAL_PROTECT  13
#define  INT_PAGE_FAULT       14

#define  INT_X87_FPU          16
#define  INT_ALIGNMENT        17
#define  INT_MACHINE_CHECK    18
#define  INT_SIMD_FLOAT       19
#define  INT_VIRTUAL_EXEC     20 

void isr0();             /*#DE div zero*/
void isr1();             /*#DB debug fault*/
void isr2();             /*NMI*/ 
void isr3();             /*BP*/
void isr4();             /*OF overflow_error*/
void isr5();             /*BR out_of_range*/
void isr6();             /*UD err_oper_code*/ 
void isr7();             /*NM math cpu*/
void isr8();             /*DF double fault*/
void isr9();             /*jump segment*/
void isr10();            /*TS TSS error*/ 
void isr11();            /*NP SEGMENT is not exist*/  
void isr12();            /*SS stack fault*/  
void isr13();            /*GP normal protect*/
void isr14();            /*FP page fault*/
void isr15();            /*CPU save*/
void isr16();            /*MF float fault*/
void isr17();            /*AC aligned*/
void isr18();            /*MC machine check*/
void isr19();            /*XM SIMD*/

/*inter save*/

void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

/*32~255 belongs user*/

void isr128();

/*IRQ handle fun()*/

void irq_handler(pt_regs_t *regs);

#define IRQ0        32
#define IRQ1        33
#define IRQ2        34
#define IRQ3        35
#define IRQ4        36
#define IRQ5        37
#define IRQ6        38
#define IRQ7        39
#define IRQ8        40
#define IRQ9        41
#define IRQ10       42
#define IRQ11       43
#define IRQ12       44
#define IRQ13       45
#define IRQ14       46
#define IRQ15       47

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

void idt_init(void);

#endif
