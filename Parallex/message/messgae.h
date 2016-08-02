/*************************************************************************
	> File Name: messgae.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 17时36分42秒
 ************************************************************************/

#ifndef _MESSGAE_H
#define _MESSGAE_H

#include "types.h"
#include "list.h"


typedef struct messgae{

    char abj[12];
    char bbj[12];
    char cbj[12];
    
    struct message * next;

}message;





#endif
