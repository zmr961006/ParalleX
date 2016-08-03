/*************************************************************************
	> File Name: char_dev.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 21时22分54秒
 ************************************************************************/

#ifndef _CHAR_DEV_H
#define _CHAR_DEV_H

#include"types.h"

typedef struct char_dev{
    const char *name;
    uint32 is_readable;
    uint32 is_writeable;
    struct char_ops{
            int (*init)(void);
            uint32 (*device_valid)(void);
            const char * (*get_desc)(void);
            int (*read)(void *,uint32);
            int (*write)(const void *,uint32);
            int (*ioctl)(int,int);
    }ops;
    struct char_dev *next;
}char_dev_t;


extern char_dev_t *char_devs;

void char_dev_init(void);

int add_char_dev(char_dev_t *cdev);

extern char_dev_t kboard_dev;

#endif
