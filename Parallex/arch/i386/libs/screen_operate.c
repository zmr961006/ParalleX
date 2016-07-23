/*************************************************************************
	> File Name: screen_operate.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 10时43分41秒
 ************************************************************************/

#include "screen_operate.h"
#include "common.h"

static int16 * screen_point = (int16 *)(0xb8000);

static int16 screen_abscissa = 0;
static int16 screen_ordinate = 0;

static void move_point(){
    
    uint16 loction = screen_abscissa *80 + screen_ordinate;

    outb(0x3D4,14);
    outb(0x3D5,loction >> 8);
    outb(0x3D4,15);
    outb(0x3D5,loction);


}

void screen_clear(){
    
    int16 back_white = ((screen_black | screen_white ) << 8) | 0x20;

    int i = 0;
    for(i = 0;i < 80 * 25;i++){
        screen_point[i] = back_white;
    }
    screen_abscissa = 0;
    screen_ordinate = 0;

    move_point();

}

void screen_char(char c,color back_color,color fore_color){
    
    int16 color_s = ((back_color | fore_color) << 8) | ((int16)c);

    if(c == '\n'){
        screen_abscissa++;
        screen_ordinate = 0;

    }else if(c == 0x09){
        screen_ordinate = screen_ordinate + 8;
    }else if(c == ' '){
        screen_ordinate++;
    }else{
        screen_point[screen_abscissa * 80 + screen_ordinate] = color_s;
        screen_ordinate++;
    }

    if(screen_ordinate > 80){
        screen_abscissa++;
        screen_ordinate = 0;
    }

    move_point();

}

void screen_string(int8 *string,color back_color,color fore_color){
    
    int i = 0;
    while(string[i] != '\0'){
        screen_char(string[i++],back_color,fore_color);
    }

}
