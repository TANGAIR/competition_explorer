/*************************************************************
  *文件名称：TIM定时器外设功能源文件
	
	*数据定义：无
	
  *外部数据使用：无
	
	*函数定义：TIM2定时器初始化函数、
	           TIM2定时器中断服务函数
	
  *外部自定义函数使用：遥控器数据转换函数
	                     电机控制函数
											 CAN1数据发送函数
  
	*功能说明：开启tIm2定时器进行定时，使用定时器中断定时运行控制函数，
***************************************************************/


/****************头文件包含区******************************/
#include "main.h"

/****************数据定义区********************************/
int TIMtimer=1;//用于TM2中段
int TimingDelay=0;//用于延迟函数
int GetTick=0;//用于计算系统运行时间
/****************函数功能区******************************/
/*
  *函数名称：TIM2定时器初始化函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无
  *功能说明：将TIM2初始化,开启定时器，中断产生频率设置，中断优先级设置，尽量设低
*/
void  TIM2_Init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure; 
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 开启TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 100-1;       
	
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 900-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	// 清除定时器更新中断标志位
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	// 开启定时器更新中断
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	// 使能定时器
	TIM_Cmd(GENERAL_TIM, ENABLE);	
	
	
	
}


/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
	
}

void SysTick_Handler(void)
{
	GetTick++;
 if(TimingDelay>0)
		TimingDelay--; 
}
/*
  *函数名称：TIM2定时器中断服务函数
  *参数输入：无
  *返回值：  无
  *硬件信息：无
  *功能说明：每隔一个固定的时间运行一次控制函数
*/

void TIM2_IRQHandler(void)//每1ms产生一次中断
{
if ( TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET ) 
	{	

		TIMtimer++;
		if(TIMtimer%10==0&&PID_track_Flag==1&&IMU_track_right==0)
		{
			
			//Chassis_Control(SPEED_RIOT*PID_left,SPEED_RIOT*PID_right);
			PID_track(PID_left,PID_right);
      CAN1_TX();
		}
		IMU_Get_Data();
		
	  if(TIMtimer%10==0&&IMU_track_right!=0&&PID_track_Flag==0)//
		{
			 
		  IMU_track(IMU_track_left,IMU_track_right);
      CAN1_TX();
		}
		
		
		if(TIMtimer%1000==0)
		{	
		 TIMtimer=1;
		}
		 TIM_ClearITPendingBit (TIM2,TIM_IT_Update);
	}			 
}






