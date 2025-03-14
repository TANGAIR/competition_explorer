#include "main.h"

int i=0;

void task(void)
{
		
	IMU_mode(310,300,0);//前
	/*一、上下坡*/	
  //通过上下坡
	while(IMU_Real_Data.Pitch>-15)//抬头-30
	{}//前段
		
		IMU_mode(310,300,0);//上加平
	 delay_ms(500);
	while(IMU_Real_Data.Pitch<15)
	{}	//到下坡
		IMU_mode(410,400,0);//下
		delay_ms(500);
	while(IMU_Real_Data.Pitch>15)//下30
	{}	//下完
	
	/*二、过隧道*/
  IMU_mode(310,310,0)	
  delay_ms(2000);
  while(HongWai==1)
  {
  }		
		PID_mode(250,270)//隧道
		  delay_ms(2000);
	//直到红外传感器没有检测到通道
	i=0;
	while(HongWai==0)
	{
		i++;
		 delay_ms(1);
		 if(i>=3000)
		 {
			 i=0;
			 Stop();
			 for(int k=0;k<=15;k++)
			 {
			 Chassis_Control(-16*120,-16*70);
			  CAN1_TX();
				delay_ms(10);
			 }
			 Stop();
			 delay_ms(700);
		 }
		 PID_mode(190,205)	
	}
	IMU_mode(150,50,195);
	//出隧道
	 delay_ms(2500);//imu
	//delay_ms(1200);//pid
	IMU_mode(200,200,180);//imu
	//PID_mode(200,205)//pid	
	
	
	 delay_ms(200);
 
	/*三、过阶梯*/
	//卡在第2节阶梯处 
	while(IMU_Real_Data.Pitch>-15)
	{}//直到检测到有上阶梯
	delay_ms(600);
	
	IMU_mode(60,55,180)
  delay_ms(2000);
	
	IMU_mode(285,300,180)
  delay_ms(300);
	Steering_OUT//伸出750
	delay_ms(1100);

 //上第2节阶梯
	Beep_ON();
	while(IMU_Real_Data.Pitch<-15)//-30
	{}	
		delay_ms(1000);	
	//下阶梯
	//下阶梯之后修正
	IMU_mode(100,120,182)
	delay_ms(1000);
  Beep_OFF();	
		
	//寻迹准备进下一个隧道
	PID_mode(260,250)	
	 delay_ms(1000);	
		
		
		
	IMU_mode(2,2,180);//
	Steering_IN
	delay_ms(1000);
		
	PID_mode(200,200)		
	while(HongWai==1)
   {}	 
	/*四、过隧道*/
  //开启慢速PID模式，经过隧道
	 PID_mode(200,200)	
	 delay_ms(2000);
	//直到红外传感器没有检测到通道
	while(HongWai==0)
   {
		 i++;
		 delay_ms(1);
		 if(i>=3000)
		 {
			 i=0;
			 Stop();
			 for(int k=0;k<=20;k++)
			 {
			 Chassis_Control(-16*70,-16*120);
			  CAN1_TX();
				delay_ms(10);
			 }
			 Stop();
			 delay_ms(700);
		 }
		 PID_mode(190,205)	
	 }
	 IMU_mode(100,100,0)
	  delay_ms(1500);
	 PID_mode(200,200)	
	  delay_ms(2000);
/*五、过栅格*/
	//经过三秒
	 IMU_mode(435,450,0)	//三个参数分别是左轮速度，右轮速度以及正对角度
	 delay_ms(3500);
	
		
	/*六、到达终点、停车*/	
  //下坡后使用PID准确到达终点
	IMU_mode(300,300,0);
		delay_ms(3000);	

	IMU_mode(2,2,0);;//在终点之后停车
		
		
}
	


















