#include "main.h"


 void steering_init(void)//�����ʼ��
 {
 //����GPIO��
//PWM������� �����Ҷ��
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
//�����
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
/* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
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
 


 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
