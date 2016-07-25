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
#include "debug.h"

/*物理起始与结束地址*/

//static uint32 pmm_addr_start;   

//static uint32 pmm_addr_end;


//static void get_ram_info(e820map_t *e820map);


void pmm_init(void){

    show_kernel_memory_map();   /*展示内核所处物理地址*/

    show_memory_map();
}



