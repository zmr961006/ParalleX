/*************************************************************************
	> File Name: string.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月22日 星期五 11时09分00秒
 ************************************************************************/

#ifndef _STRING_H
#define _STRING_H

#include "types.h"

void  *memcpy(void *dest,const void *src,uint32 len);

void  memset(void *dest,int8 val,uint32 len);

void  bzer(void *dest,uint32 len);

int   strcmp(const int8 *dest,const int8 *val);

int8  *strcpy(char *dest,const char *val);

int8  *strcat(int8 * dest,const int8 *val);

int   strlen(const int8 *src);
#endif
