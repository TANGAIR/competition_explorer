#include "main.h"

int i=0;

void task(void)
{
		
	IMU_mode(310,300,0);//ǰ
	/*һ��������*/	
  //ͨ��������
	while(IMU_Real_Data.Pitch>-15)//̧ͷ-30
	{}//ǰ��
		
		IMU_mode(310,300,0);//�ϼ�ƽ
	 delay_ms(500);
	while(IMU_Real_Data.Pitch<15)
	{}	//������
		IMU_mode(410,400,0);//��
		delay_ms(500);
	while(IMU_Real_Data.Pitch>15)//��30
	{}	//����
	
	/*���������*/
  IMU_mode(310,310,0)	
  delay_ms(2000);
  while(HongWai==1)
  {
  }		
		PID_mode(250,270)//���
		  delay_ms(2000);
	//ֱ�����⴫����û�м�⵽ͨ��
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
	//�����
	 delay_ms(2500);//imu
	//delay_ms(1200);//pid
	IMU_mode(200,200,180);//imu
	//PID_mode(200,205)//pid	
	
	
	 delay_ms(200);
 
	/*����������*/
	//���ڵ�2�ڽ��ݴ� 
	while(IMU_Real_Data.Pitch>-15)
	{}//ֱ����⵽���Ͻ���
	delay_ms(600);
	
	IMU_mode(60,55,180)
  delay_ms(2000);
	
	IMU_mode(285,300,180)
  delay_ms(300);
	Steering_OUT//���750
	delay_ms(1100);

 //�ϵ�2�ڽ���
	Beep_ON();
	while(IMU_Real_Data.Pitch<-15)//-30
	{}	
		delay_ms(1000);	
	//�½���
	//�½���֮������
	IMU_mode(100,120,182)
	delay_ms(1000);
  Beep_OFF();	
		
	//Ѱ��׼������һ�����
	PID_mode(260,250)	
	 delay_ms(1000);	
		
		
		
	IMU_mode(2,2,180);//
	Steering_IN
	delay_ms(1000);
		
	PID_mode(200,200)		
	while(HongWai==1)
   {}	 
	/*�ġ������*/
  //��������PIDģʽ���������
	 PID_mode(200,200)	
	 delay_ms(2000);
	//ֱ�����⴫����û�м�⵽ͨ��
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
/*�塢��դ��*/
	//��������
	 IMU_mode(435,450,0)	//���������ֱ��������ٶȣ������ٶ��Լ����ԽǶ�
	 delay_ms(3500);
	
		
	/*���������յ㡢ͣ��*/	
  //���º�ʹ��PID׼ȷ�����յ�
	IMU_mode(300,300,0);
		delay_ms(3000);	

	IMU_mode(2,2,0);;//���յ�֮��ͣ��
		
		
}
	


















