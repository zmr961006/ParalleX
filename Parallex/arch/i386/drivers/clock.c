/*************************************************************************
	> File Name: clock.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月23日 星期六 14时33分52秒
 ************************************************************************/

#include "common.h"
#include "clock.h"
#include "intr.h"
#include "vagr.h"

#define  FREQUENCY      100

#define  IO_TIMER       0x040

#define  TIMER_FREQ     1193180

#define  TIMER_MODE     (IO_TIMER + 3)

#define  TIMER_SEL0     0x00

#define  TIMER_RATEGEN  0x04 

#define  TIMER_CLK      0x06 

#define  TIMER_16BIT    0x30 

void clock_callback();

void clock_callback(){

    printk("I am the time\n");

}


void clock_init(void){
    
        register_interrupt_handler(IRQ0,clock_callback);

        uint32 divisor = TIMER_FREQ / FREQUENCY;

        outb(TIMER_MODE,TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);

        uint8 low = (uint8)(divisor & 0xFF);
        uint8 high = (uint8)((divisor >> 8) & 0xFF);

        outb(IO_TIMER,low);
        outb(IO_TIMER,high);

}

