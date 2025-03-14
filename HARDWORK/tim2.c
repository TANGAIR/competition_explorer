/*************************************************************
  *�ļ����ƣ�TIM��ʱ�����蹦��Դ�ļ�
	
	*���ݶ��壺��
	
  *�ⲿ����ʹ�ã���
	
	*�������壺TIM2��ʱ����ʼ��������
	           TIM2��ʱ���жϷ�����
	
  *�ⲿ�Զ��庯��ʹ�ã�ң��������ת������
	                     ������ƺ���
											 CAN1���ݷ��ͺ���
  
	*����˵��������tIm2��ʱ�����ж�ʱ��ʹ�ö�ʱ���ж϶�ʱ���п��ƺ�����
***************************************************************/


/****************ͷ�ļ�������******************************/
#include "main.h"

/****************���ݶ�����********************************/
int TIMtimer=1;//����TM2�ж�
int TimingDelay=0;//�����ӳٺ���
int GetTick=0;//���ڼ���ϵͳ����ʱ��
/****************����������******************************/
/*
  *�������ƣ�TIM2��ʱ����ʼ������
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ����
  *����˵������TIM2��ʼ��,������ʱ�����жϲ���Ƶ�����ã��ж����ȼ����ã��������
*/
void  TIM2_Init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure; 
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// ����TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 100-1;       
	
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 900-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// ��ʼ����ʱ��TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	// �����ʱ�������жϱ�־λ
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	// ������ʱ�������ж�
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	// ʹ�ܶ�ʱ��
	TIM_Cmd(GENERAL_TIM, ENABLE);	
	
	
	
}


/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
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
  *�������ƣ�TIM2��ʱ���жϷ�����
  *�������룺��
  *����ֵ��  ��
  *Ӳ����Ϣ����
  *����˵����ÿ��һ���̶���ʱ������һ�ο��ƺ���
*/

void TIM2_IRQHandler(void)//ÿ1ms����һ���ж�
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






