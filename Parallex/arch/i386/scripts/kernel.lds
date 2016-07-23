ENTRY(start)  			/*ENTRY(symbol),指定symbol就是链接器指定的内核的入口*/

SECTIONS {
	. = 0x100000; 	 	
		/* "."的代表当前的虚拟地址，赋值语句的意思是将当前虚拟地址设置成0x10000		
		*为什么0x100000,grub从elf内核文件中读取物理地址（加载地址LMA），这个值    
		*必须大于或等于1M，并且小于真实的物理地址RAM的大小。这是grub的限制，并不是
		*多启动机制的限制
		*/
	.text : {
		*(.text) 
		. = ALIGN(4096); 
	/*	段设置为4kb对齐 */
	}

	.data : {
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	
	.bss :{
		*(.bss)
		. = ALIGN(4096);
	}
	
	.stab :{ 		/*调试信息*/
		*(.stab)
		. = ALIGN(4096);
	}
	
	.stabstr : { 		/*.stab中用到的字符串*/
		*(.stabstr)
		. = ALIGN(4096);
	}

}
