/*************************************************************************
	> File Name: printk.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 11时36分56秒
 ************************************************************************/

#include "vagr.h"
#include "types.h"
#include "string.h"
#include "screen_operate.h"
#include "common.h"


static void printchar(int8 str);
static int vprintk(const int8 * format,char *ap);
static void printi(int number);
static void prints(char *s);
static void printx(int number);

void printx(int number){

    char *hex = "0123456789ABCDEF";
    int value;
    int i = 7;
    while(i >= 0){
        value = number >> i*4;
        screen_string_color(hex[value & 0xF],screen_black,screen_red);
        i--;
    }


}

void prints(char *s){  
    
    screen_write_color(s , screen_black, screen_red);
    
}


void printchar(int8 str){
    
    screen_string_color(str,screen_black,screen_red);
}


void printi(int number){
    
    char s[32];
    int sign;
    int i = 0;
    sign = number;

    if(sign < 0){
        number = -number;
    }
    do{
        s[i++] = number % 10 + '0';
    }while((number /= 10) > 0);

    if(sign < 0){
        s[i++] = '-';
    }
    i = i - 1;
    for(; i >= 0;i--){
        screen_string_color(s[i],screen_black,screen_red);
    }

}

int printk(const int8 * format,...){
    int number;
    char *arg;
    va_start(arg,format);
    number = vprintk(format,arg);
    va_end(arg);
    return number;

}


int vprintk(const int8 * format,char *ap){
    
    int pc;
    pc = 0;
    for(;*format != '\0';++format){
        if(*format == '%'){
            ++format;
            if((*format) == 'c'){
                printchar(va_arg(ap,int));
                pc++;
            }else if(*format == 'd'){
                printi(va_arg(ap,int));
                pc++;
            }else if(*format == 's'){
                prints((va_arg(ap,char*)));
                pc++;
            }else if(*format == 'x'){
                printx((va_arg(ap,int)));
                pc++;
            }else{
                printchar(*format);
                pc++;
            }
        }else{
            printchar(*format);
            pc++;
        }
    }
    return pc;

}
