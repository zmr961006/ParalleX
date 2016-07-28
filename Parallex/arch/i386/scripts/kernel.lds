ENTRY(start)  			/*ENTRY(symbol),指定symbol就是链接器指定的内核的入口*/

SECTIONS {
	. = 0x100000; 	 	
		/* "."的代表当前的虚拟地址，赋值语句的意思是将当前虚拟地址设置成0x10000		
		*为什么0x100000,grub从elf内核文件中读取物理地址（加载地址LMA），这个值    
		*必须大于或等于1M，并且小于真实的物理地址RAM的大小。这是grub的限制，并不是
		*多启动机制的限制
		*/
    PROVIDE(kernel_start = . );
    PROVIDE(kernel_init_text_start = .);
    .init.text :
    {
        *(.init.text)
        . = ALIGN(4096);
    }
    
    PROVIDE(kernel_init_text_end = .);
    PROVIDE(kernel_init_data_start = .);
    .init.data :
    {
        *(.init.data)
        . = ALIGN(4096);
    }
    PROVIDE(kernel_init_data_end = .);

    . += 0xC0000000;

    PROVIDE(kernel_text_start = . - 0xC0000000 );
	.text : AT(ADDR(.text) - 0xC0000000)
    {
		*(.text) 
		. = ALIGN(4096); 
	/*	段设置为4kb对齐 */
	}

    PROVIDE(kernel_text_end = . - 0xC0000000);
    PROVIDE(kernel_data_start = . - 0xC0000000);

	.data : AT(ADDR(.data) - 0xC0000000)
    {
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	
	.bss :  AT(ADDR(.bss) - 0xC0000000)
    {
		*(.bss)
		. = ALIGN(4096);
	}
	
    PROVIDE(kernel_data_end = . -0xC0000000);

	.stab : AT(ADDR(.stab) - 0xC0000000)
    { 		/*调试信息*/
		*(.stab)
		. = ALIGN(4096);
	}
	
	.stabstr : AT(ADDR(.stabstr) - 0xC0000000) 
    { 		/*.stab中用到的字符串*/
		*(.stabstr)
		. = ALIGN(4096);
	}

    PROVIDE(kernel_end = . - 0xC0000000);
    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
