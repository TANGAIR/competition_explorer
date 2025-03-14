#include "main.h"


 void steering_init(void)//舵机初始化
 {
 //设置GPIO口
//PWM输出设置 ，左右舵机
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
//主舵机
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//TIM10 ????
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); //?? PORTF ??

GPIO_PinAFConfig(GPIOI,GPIO_PinSource0,GPIO_AF_TIM5);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOF6
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //????
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //?? 50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
GPIO_Init(GPIOI,&GPIO_InitStructure); //??? PF9
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
/* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 84-1;	
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//?????? 14
//??? TIM14 Channel1 PWM ??
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM ???? 1
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //?????


TIM_OC4Init(TIM5, &TIM_OCInitStructure); //????? TIM1 4OC1
TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); //????????

TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE ??
TIM_Cmd(TIM5, ENABLE); //?? TIM11

 }
 


 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
