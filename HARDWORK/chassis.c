/*************************************************************
  *文件名称：底盘控制源文件
	
	*数据定义：底盘4个电机的速度
	
  *外部数据使用：遥控器控制信息数据结构体、
	               电机CAN数据结构体、
								 电机PID数据结构体
	
	*函数定义：PID初始化函数、
	           PID计算函数
	
  *外部自定义函数使用： PID计算函数
  
	*功能说明：通过pid将遥控器控制信息转化为电机速度
***************************************************************/


/****************头文件包含区******************************/
#include "main.h"
/****************数据定义区********************************/
/*电机速度数据定义*/
int   LeftFront_Speed=0,
      RightBack_Speed=0;  
int PID_LEVEL_1=7,
    PID_LEVEL_2=8,
    PID_LEVEL_3=10;
int PID_left=0,
    PID_right=0;
/****************函数功能区******************************/
/*前进*/

void Chassis_Control(int left_speed,int right_speed )
{
		 LeftFront_Speed  =left_speed;//0x201
		
		 RightBack_Speed  =-right_speed;//0x203
    	
		 CAN_DATA_Left.Target_Speed=PID_Calc(&PID_Left,CAN_DATA_Left.Real_Speed,LeftFront_Speed);//201
	
		 CAN_DATA_Right.Target_Speed=PID_Calc(&PID_Right,CAN_DATA_Right.Real_Speed,RightBack_Speed); //203
	 
			
}
/*停止*/
void Stop(void)
{

     CAN_DATA_Left.Target_Speed=0;
		 CAN_DATA_Right.Target_Speed=0;
	
	   PID_track_Flag=0;
     IMU_track_right=0;

}


//PID直线前进循迹
void PID_track(float pwm_left, float pwm_right)
{
  int count=0;
	float LEFT=pwm_left;
	float RIGHT=pwm_right;
		
  float error=0;
 static float lasterror=0;
 
  float kp=100;//决定每次调节幅度的大小,数值越大调节幅度越大            
  float ki=0.0f;      
  float kd=10; //可以决定调节速度的快慢，数值越大调节速度越快,当以比例为主的时候. 可以起到修正作用         
  float derivative=0; 
  static  float iout=0;	
  float pid=0;
	
	
	int a;
  a=((F_L2 << 3) | (F_L1 << 2) | (F_R1 << 1) | (F_R2));         
   
  switch(a)                     
  { 
   case 0x07:  //0111
   error=0;   
   break; 
	 
	  case 0x0e:  //1110
   error=0;   
   break;
		
	

  case 0x00:    // 0000 
  error=0; 
  break; 
  
  case 0x06:    // 0110 
  error=0; 
  break; 
  
  case 0x09:    // 1001 
  error=0;
  break; 
  
  case 0x02:    // 0010 
  error=PID_LEVEL_1+1;
  break; 

  case 0x03:    // 0011
  error=PID_LEVEL_2+1; 
  break; 

  case 0x01:    //0001
  error=PID_LEVEL_3+1;
  break; 
    
  case 0x04:    //0100 
  error=-PID_LEVEL_1;
  break; 
    
  case 0x0c:    // 1100 
  error=-PID_LEVEL_2;                 
  break; 
  
  case 0x08:    //1000 
  error=-PID_LEVEL_3;
  break;
	 
   default:
	 error=0;
   break; 
  }
	lasterror=error;
	
//	if(B_L2==0)//当中间的传感器没有检测到黑线的时候
//	{
//	count=1000;
//	LEFT=100;
//	RIGHT=100;	
//	}
//	
//	if(count>0&&B_L2==1)//当中间的两个传感器检测到黑线的时候,
//	{
//	  count--;
//		delay_ms(1);
//		LEFT=pwm_left/8;
//		RIGHT=pwm_right/8;
//	}
//	
  iout += ki *error;
  derivative=error-lasterror;	
  pid=kp*error+kd*derivative+ki*iout;
	pid=pid/10;

  Chassis_Control(SPEED_RIOT*(LEFT+pid),SPEED_RIOT*(RIGHT-pid));		
}





//IMU直线前进循迹
void IMU_track(float pwm_left, float pwm_right)
{
	float LEFT=pwm_left;
	float RIGHT=pwm_right;
		
  float error=0;
  static  float lasterror=0;
 
  float kp=50;//决定每次调节幅度的大小,数值越大调节幅度越大            
  float ki=0.0f;      
  float kd=0; //可以决定调节速度的快慢，数值越大调节速度越快,当以比例为主的时候. 可以起到修正作用         
  float derivative=0; 
  static  float iout=0;	
  float pid=0;
	
	if(fabs(IMU_Real_Data .YAW)<=0.5)
	IMU_Real_Data .YAW=0;
	
	if(IMU_Real_Data .YAW<=-90)
	IMU_Real_Data .YAW+=360;
	
  error=IMU_Real_Data.YAW-IMU_direction_angle;
	lasterror=error;

	
  iout += ki *error;
  derivative=error-lasterror;	
 
  pid=kp*error+kd*derivative+ki*iout;
	pid=pid/5;
	
  
  Chassis_Control(SPEED_RIOT*(LEFT+pid),SPEED_RIOT*(RIGHT-pid));		
}





















