#ifndef __TIM2_H
#define __TIM2_H 	
#include "sys.h"


/***************宏定义区*********************/
#define GENERAL_TIM           		TIM2
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn		      TIM2_IRQn
#define GENERAL_TIM_IRQHandler    TIM2_IRQHandler


/*******************函数声明************************/
void TIM2_Init(void);
void SysTick_Init(void);
extern int TIMtimer;
extern int TimingDelay;
extern int GetTick;



#endif


