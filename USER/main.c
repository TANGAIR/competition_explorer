#include "main.h"
char PID_track_Flag=0;
short IMU_track_right=0;
short IMU_track_left=0;
short IMU_direction_angle=0;

int main(void)//主函数
{
	/*程序中断优先级分组*/
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );//整个程序的中断分组为二
  /*初始化函数*/
	Beep_Init();                     //蜂鸣器初始化
	LED_Init ();                     //LED灯初始化
	delay_init (180);                //延迟函数初始化
	USART6_Init();                   //串口六初始化
	CAN1_Init();                     //CAN1初始化
	PID_Init();                      //PID电机参数初始化
	SysTick_Init();
  IMU_Init();                      //陀螺仪初始化函数
	gray_sensor_init();              //灰度传感器初始化
	steering_init();                 //舵机初始化
	
	
	
	led_against();                   //绿灯闪烁后常亮，蜂鸣器响一声，之后绿灯常亮
  TIM2_Init();                     //TIM2定时器初始化
 /***********************初始化结束*******************************/	 
	printf("\\——————————————Init OK——————————————————//\r\n");
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
// IMU_mode(470,450,0)	//三个参数分别是左轮速度，右轮速度以及正对角度
//  delay_ms(3000);
//	IMU_mode(470,450,180)
//		 delay_ms(3000);
	} 
 
		
}













