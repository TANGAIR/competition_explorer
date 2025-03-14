#include "main.h"

void Beep_Init()
{
   GPIO_InitTypeDef          GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;  
	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	  
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource6,GPIO_AF_TIM12); 
	  
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOH,&GPIO_InitStruct);
	  
	  TIM_TimeBaseInitStructure.TIM_Prescaler=90-1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=400-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;	    
	  TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);
 
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	  TIM_OC1Init(TIM12,&TIM_OCInitStructure);
	
	  TIM_OC1PreloadConfig(TIM12,TIM_OCPreload_Enable);
	  TIM_ARRPreloadConfig(TIM12,ENABLE);
	  TIM_Cmd(TIM12,ENABLE);
}
void Beep_ON(void)
{
 TIM_SetCompare1(TIM12,200);	
}


void Beep_OFF(void)
{
 TIM_SetCompare1(TIM12,0);	
}
