#ifndef _PID_H
#define _PID_H

#include "sys.h"


/*******************结构体声明************************/
typedef struct
{
	float P;
	float I;
  float D;
	float CurrentError;
	float LastError;
	float ErrorIgnore;
	float Pout;
	float Iout;
	float Dout;
	float PIDout;
	float PIDoutCompensate;
	float PMax;
	float IMax;
	float DMax;
	float PIDOutMax;
	float PIDOutLast;
	float Target_Speed_Last;
	float Speed_Ratio;
	float Acceleration;			
}PID_TypeDef;


/*******************函数声明************************/
void PID_Init(void);
float PID_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Speed);

/**************************数据作用域拓展********************************/
extern PID_TypeDef       
	                //底盘电机四个M3508
                  PID_Left,  //201左前轮
									PID_Right;  //203右后轮










#endif

