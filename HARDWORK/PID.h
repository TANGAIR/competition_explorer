#ifndef _PID_H
#define _PID_H

#include "sys.h"


/*******************�ṹ������************************/
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


/*******************��������************************/
void PID_Init(void);
float PID_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Speed);

/**************************������������չ********************************/
extern PID_TypeDef       
	                //���̵���ĸ�M3508
                  PID_Left,  //201��ǰ��
									PID_Right;  //203�Һ���










#endif

