/*************************************************************************
	> File Name: kio.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 21时30分49秒
 ************************************************************************/

#include "common.h"
#include "char_dev.h"
#include "kio.h"

char getchar(void){

        char ch;
        char_dev_t *kb_dev = &kboard_dev;
        if(!kb_dev->ops.device_valid()){
            return 0;
        }
        while(kb_dev->ops.read(&ch,1) == 1){
                cpu_hlt();
                return ch;
        }
    return 0;
}
