/*************************************************************************
	> File Name: intr.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 10时04分17秒
 ************************************************************************/

#include"common.h"
#include"types.h"
#include"intr.h"
#include"vagr.h"
#include"pic.h"

#define INTERRUPT_MAX  255

idt_entry_t idt_entries[INTERRUPT_MAX] __attribute__ ((aligned(16)));

idt_ptr_t  idt_ptr;

interrupt_handler_t interrupt_handlers[INTERRUPT_MAX] __attribute__ ((aligned(4)));

void idt_set_gate(uint8 num,uint32 base,uint16 sel,uint8 flags);

extern uint32 idt_flush(uint32);

typedef void (*isr_irq_func_t)();

isr_irq_func_t isr_irq_func[INTERRUPT_MAX] = {
    
    [0] = &isr0,[1] = &isr1,[2] = &isr2,[3] = &isr3,
    [4] = &isr4,[5] = &isr5,[6] = &isr6,[7] = &isr7,
    [8] = &isr8,[9] = &isr9,[10] = &isr10,[11] = &isr11,
    [12] = &isr12,[13] = &isr13,[14] = &isr14,[15] = &isr15,
    [16] = &isr16,[17] = &isr17,[18] = &isr18,[19] = &isr19,
    [20] = &isr20,[21] = &isr21,[22] = &isr22,[23] = &isr23,
    [24] = &isr24,[25] = &isr25,[26] = &isr26,[27] = &isr27,
    [28] = &isr28,[29] = &isr29,[30] = &isr30,[31] = &isr31,

    
    [32] = &irq0,[33] = &irq1,[34] = &irq2,[35] = &irq3,
    [36] = &irq4,[37] = &irq5,[38] = &irq6,[39] = &irq7,
    [40] = &irq8,[41] = &irq9,[42] = &irq10,[43] = &irq11,
    [44] = &irq12,[45] = &irq13,[46] = &irq14,[47] = &irq15,
};


void idt_init(void){
    
    init_interrupt_chip();
    idt_ptr.limit = sizeof(idt_entry_t) * INTERRUPT_MAX - 1;
    idt_ptr.base  = (uint32)&idt_entries;

    for(uint32 i = 0;i < 48;i++){
        idt_set_gate(i,(uint32)isr_irq_func[i],0x08,0x8E);
    }
    idt_set_gate(128,(uint32)isr128,0x08,0xEF);

    idt_flush((uint32)&idt_ptr);

}


void idt_set_gate(uint8 num,uint32 base,uint16 sel,uint8 flags){
    
    idt_entries[num].base_low = base & 0XFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;

    idt_entries[num].flags = flags;

}

const char * intrname(uint32 intrno){
    
    static const char * const intrnames[] = {
        "Divide error",
        "Debug",
        "Non-Maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "Bound range Exceeded",
        "Invalid Opcode",
        "Decvice Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection",
        "Page Fault",
        "(UNkonw trap)",
        "X87 FPU Floating ERROR",
        "alignment Check",
        "Machine-Check",
        "SIMD Floating Execption"
    };

    if(intrno < sizeof(intrnames)/sizeof(const char *const)){
        return intrnames[intrno];
    }

    return "(uknow trap)";

}

void isr_handler(pt_regs_t *regs){
    
    if(interrupt_handlers[regs->int_no]){
        interrupt_handlers[regs->int_no](regs);
    }else{
        printk("unable interrupt %d %s\n",regs->int_no,intrname(regs->int_no));
        cpu_hlt();
    }

}

void register_interrupt_handler(uint8 n,interrupt_handler_t h){
    interrupt_handlers[n] = h;
}

void irq_handler(pt_regs_t *regs){

    clear_interrupt_chip(regs->int_no);
    if(interrupt_handlers[regs->int_no]){
        interrupt_handlers[regs->int_no](regs);
    }

}
