#include "main.h"
char PID_track_Flag=0;
short IMU_track_right=0;
short IMU_track_left=0;
short IMU_direction_angle=0;

int main(void)//������
{
	/*�����ж����ȼ�����*/
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );//����������жϷ���Ϊ��
  /*��ʼ������*/
	Beep_Init();                     //��������ʼ��
	LED_Init ();                     //LED�Ƴ�ʼ��
	delay_init (180);                //�ӳٺ�����ʼ��
	USART6_Init();                   //��������ʼ��
	CAN1_Init();                     //CAN1��ʼ��
	PID_Init();                      //PID���������ʼ��
	SysTick_Init();
  IMU_Init();                      //�����ǳ�ʼ������
	gray_sensor_init();              //�Ҷȴ�������ʼ��
	steering_init();                 //�����ʼ��
	
	
	
	led_against();                   //�̵���˸��������������һ����֮���̵Ƴ���
  TIM2_Init();                     //TIM2��ʱ����ʼ��
 /***********************��ʼ������*******************************/	 
	printf("\\����������������������������Init OK������������������������������������//\r\n");
  Steering_IN
 
 //PID_mode(220,200)	

	task();

	while(1)
	{
//		if(HongWai==0)
//		{
//	     Beep_ON();	
//		}
//		else
//    {
//			Beep_OFF();	
//		}
//		delay_ms(10);
// IMU_mode(470,450,0)	//���������ֱ��������ٶȣ������ٶ��Լ����ԽǶ�
//  delay_ms(3000);
//	IMU_mode(470,450,180)
//		 delay_ms(3000);
	} 
 
		
}













