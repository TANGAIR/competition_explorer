#ifndef __STEERING_H
#define __STEERING_H 			   
#include "sys.h"	

     
#define	Steering_OUT           TIM_SetCompare4(TIM5,750);
#define	Steering_IN            TIM_SetCompare4(TIM5,2450);	 

 void steering_init(void);//¶æ»ú³õÊ¼»¯
 

#endif
