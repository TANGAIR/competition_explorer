/*************************************************************
  *文件名称：PID源文件
	
	*数据定义：电机pId参数数据结构体
	
  *外部数据使用：主函数头文件中宏定义的PID电机参数
	
	*函数定义：PID初始化函数、
	           PID计算函数
	
  *外部自定义函数使用：无
  
	*功能说明：通过初始化传输，将各个电机的pid相关参数设置
	           提供pId计算函数
						 缺少自瞄相关的pId参数
***************************************************************/


/****************头文件包含区******************************/
#include "main.h"



/****************数据定义区********************************/
/*电机CAN数据结构体对象定义*/
//CAN1
//底盘电机四个M3508
PID_TypeDef       PID_Left,  //201左轮							
									PID_Right;  //203右轮
								 


/****************函数功能区******************************/
/*
  *函数名称：PID初始化函数
  *参数输入：无
  *返 回值：  无
  *硬件信息：无
  *功能说明：根据调试的结果，将pId参数写入各电机 PId数据结构体
*/
void PID_Init()
{
	 {//底盘PID
		 
		/*0x201左前轮*/ 
		PID_Left.P=30;
		PID_Left.I=0.5f;
		PID_Left.D=10;
		 
	
		 
		/*0x203右后轮*/
		PID_Right.P=30;
		PID_Right.I=0.5f;
		PID_Right.D=10;
	
	 }
{//底盘PID其他参数
	 
	/*0x201*/
	PID_Left.CurrentError=0;
	PID_Left.LastError=0;
	PID_Left.ErrorIgnore=2;
	PID_Left.Pout=0;
	PID_Left.Iout=0;
	PID_Left.Dout=0;
	PID_Left.PIDout=0;
	PID_Left.PIDoutCompensate=0;
	PID_Left.PMax=15000;
	PID_Left.IMax=4000;
	PID_Left.DMax=5000;
	PID_Left.PIDOutMax=20000;
	PID_Left.PIDOutLast=0;
	PID_Left.Target_Speed_Last=0;
	PID_Left.Speed_Ratio=1;
	PID_Left.Acceleration=5000;
	


	/*0x203*/
	PID_Right.CurrentError=0;
	PID_Right.LastError=0;
	PID_Right.ErrorIgnore=2;
	PID_Right.Pout=0;
	PID_Right.Iout=0;
	PID_Right.Dout=0;
	PID_Right.PIDout=0;
	PID_Right.PIDoutCompensate=0;
	PID_Right.PMax=15000;
	PID_Right.IMax=4000;
	PID_Right.DMax=5000;
	PID_Right.PIDOutMax=20000;
	PID_Right.PIDOutLast=0;
	PID_Right.Target_Speed_Last=0;
	PID_Right.Speed_Ratio=1;
	PID_Right.Acceleration=5000;
	

 }
}

/*
  *函数名称：PID计算函数
  *参数输入：PID_TypeDef *PID：电机相对应的pId参数数据结构体
             float Current_Speed：电机反馈回来的当前速度值
             float Target_Speed：经过解码转换的遥控器速度控制目标值
  *返回值:   PID->PIDout:经过pid计算以后的输出
  *硬件信息：无
  *功能说明：根据提供的电机PID参数结构体地址代入参数,结合反馈回的当前速度与要达到的目标速度
             进行计算，返回输出给电机的电流值
*/
float PID_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Speed)
{   	
	Current_Speed = Current_Speed/PID->Speed_Ratio;//减速比
	
	PID->Target_Speed_Last = Target_Speed;
	
	PID->CurrentError = Target_Speed - Current_Speed;
	if ( fabs(PID->CurrentError)< PID->ErrorIgnore )  PID->CurrentError = 0;
	
	//P  
	PID->Pout = PID->P * PID->CurrentError;
	if(PID->Pout> PID->PMax) PID->Pout= PID->PMax;
	if(PID->Pout<(-PID->PMax)) PID->Pout=(-PID->PMax);
	//I
	PID->Iout += PID->I * PID->CurrentError;
	if(PID->Iout> PID->IMax)   PID->Iout= PID->IMax;
	if(PID->Iout<(-PID->IMax)) PID->Iout=(-PID->IMax);
	//D 
	PID->Dout = PID->D * ( PID->CurrentError - PID->LastError );
	if(PID->Dout> PID->DMax) PID->Dout= PID->DMax;
	if(PID->Dout<(-PID->DMax)) PID->Dout=(-PID->DMax);
	//PID
	if ( PID->CurrentError > 0 ) 
		PID->PIDout = PID->Pout + PID->Iout + PID->Dout - PID->PIDoutCompensate;
	else  
		PID->PIDout = PID->Pout + PID->Iout + PID->Dout + PID->PIDoutCompensate;
	//限制
	if ( (fabs( PID->PIDout - PID->PIDOutLast)>PID->Acceleration)  && (fabs(Current_Speed) !=0) )    
	{
	  PID->PIDout =( PID->PIDout>PID->PIDOutLast) ?  (PID->PIDOutLast+PID->Acceleration) : (PID->PIDOutLast-PID->Acceleration);
	}
	if(PID->PIDout> PID->PIDOutMax) PID->PIDout= PID->PIDOutMax;
	if(PID->PIDout<(-PID->PIDOutMax)) PID->PIDout=(-PID->PIDOutMax);

  PID->PIDOutLast = PID->PIDout;
	PID->LastError = PID->CurrentError;
	
	return PID->PIDout/6;    
}








































