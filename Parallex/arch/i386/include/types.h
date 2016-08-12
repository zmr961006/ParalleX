/*
 * =====================================================================================
 *
 *       Filename:  types.h
 *
 *       some data types by ourself;
 *
 * =====================================================================================
 */

#ifndef TYPES_H
#define TYPES_H

#ifndef NULL
	#define NULL 0
#endif 

#ifndef TRUE
	#define TRUE 1
	#define FALSE 0
#endif 

#ifndef true 
    #define true 1
    #define false 0
#endif

#define E_UNSPECIFIED  1
#define E_BAD_PROC     2
#define E_INVAL        3
#define E_NO_MEM       4
#define E_NO_FREE_PROC 5
#define E_FAULT        6

#define MAXERROR       6


typedef unsigned int    uint32;
typedef 	 int    int32;
typedef unsigned short  uint16;
typedef 	 short  int16;
typedef unsigned char 	uint8;
typedef 	 char  	int8;

typedef unsigned long long uint64;
typedef          long long  int64;


/*time */
typedef uint32 time_t;

/*pid*/
typedef int32  pid_t;

/*pgd*/
typedef uint32 pgd_t;

/*pte*/
typedef int32  pte_t;

/*原子类型*/

typedef struct {
    volatile int counter;
}atomic_t;

/*链表*/
struct list_head {
    struct list_head *next,*prev;
};




#endif //TYPES_H
