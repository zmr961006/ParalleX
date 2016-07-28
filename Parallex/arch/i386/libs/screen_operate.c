/*************************************************************************
	> File Name: screen_operate.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 10时43分41秒
 ************************************************************************/

#include "screen_operate.h"
#include "common.h"
#include "sync.h"
#include "debug.h"
#include "vagr.h"
#include "vmm.h"
/*open page add page_offset*/

static  int16 * screen_point = (int16 *)(0xB8000 + PAGE_OFFSET) ; 


static int16 screen_abscissa = 0;
static int16 screen_ordinate = 0;


static void scrooll(void)
{
	uint8 attribute_byte = (0 << 4) | (15 & 0x0F);
	uint16 blank = 0x20 | (attribute_byte << 8);

	if (screen_abscissa >= 25) {
		int i;
		
		for (i = 0; i < 24 *80; i++) {
			screen_point[i] = screen_point[i+80];
			
		}

		for (i = 24*80; i < 25*80; i++) {
			screen_point[i] = blank;
		}

		screen_abscissa = 24;
	}
}


static void move_point()
{
	uint16 loction = screen_abscissa * 80 + screen_ordinate;

	outb(0x3D4, 14);
	outb(0x3D5, loction >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, loction);
}



void screen_clear()
{
	/*大端模式和小端的模式
	 *小端模式：大大小小
	 *内存地址是地址小的地方
	 */
	int16 back_white = ((screen_black | screen_white) << 8) | 0x20;

	int i = 0;
	for (i = 0; i < 80 * 25; i++) {
		screen_point[i] = back_white;
	}

	screen_abscissa = 0;
	screen_ordinate = 0;

	move_point();
}



 void screen_string_color(char c, color back_color, color fore_color )
{
	int16 color_s = ((back_color | fore_color) << 8) | ((int16)c);


	if (c == '\n') {
		screen_abscissa ++; 		//换行
		screen_ordinate = 0; 		//重头开始
	} else if (c == 0x09) {
		screen_ordinate = screen_ordinate + 8;
	} else if (c == ' ') {
		screen_ordinate ++;
	}else {
		screen_point[screen_abscissa * 80 + screen_ordinate] = color_s;
		screen_ordinate ++;
	}
	
	if (screen_ordinate > 80) {
		screen_abscissa ++;
		screen_ordinate = 0;
	}
	scrooll();
	move_point();
	
}



void screen_write_color(int8 *string, color back_color, color fore_color)
{
	int i = 0;
	while (string[i] != '\0') {
		screen_string_color(string[i++], back_color, fore_color);
	}
	
}

