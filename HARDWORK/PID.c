/*************************************************************
  *�ļ����ƣ�PIDԴ�ļ�
	
	*���ݶ��壺���pId�������ݽṹ��
	
  *�ⲿ����ʹ�ã�������ͷ�ļ��к궨���PID�������
	
	*�������壺PID��ʼ��������
	           PID���㺯��
	
  *�ⲿ�Զ��庯��ʹ�ã���
  
	*����˵����ͨ����ʼ�����䣬�����������pid��ز�������
	           �ṩpId���㺯��
						 ȱ��������ص�pId����
***************************************************************/


/****************ͷ�ļ�������******************************/
#include "main.h"



/****************���ݶ�����********************************/
/*���CAN���ݽṹ�������*/
//CAN1
//���̵���ĸ�M3508
PID_TypeDef       PID_Left,  //201����							
									PID_Right;  //203����
								 


/****************����������******************************/
/*
  *�������ƣ�PID��ʼ������
  *�������룺��
  *�� ��ֵ��  ��
  *Ӳ����Ϣ����
  *����˵�������ݵ��ԵĽ������pId����д������ PId���ݽṹ��
*/
void PID_Init()
{
	 {//����PID
		 
		/*0x201��ǰ��*/ 
		PID_Left.P=30;
		PID_Left.I=0.5f;
		PID_Left.D=10;
		 
	
		 
		/*0x203�Һ���*/
		PID_Right.P=30;
		PID_Right.I=0.5f;
		PID_Right.D=10;
	
	 }
{//����PID��������
	 
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
  *�������ƣ�PID���㺯��
  *�������룺PID_TypeDef *PID��������Ӧ��pId�������ݽṹ��
             float Current_Speed��������������ĵ�ǰ�ٶ�ֵ
             float Target_Speed����������ת����ң�����ٶȿ���Ŀ��ֵ
  *����ֵ:   PID->PIDout:����pid�����Ժ�����
  *Ӳ����Ϣ����
  *����˵���������ṩ�ĵ��PID�����ṹ���ַ�������,��Ϸ����صĵ�ǰ�ٶ���Ҫ�ﵽ��Ŀ���ٶ�
             ���м��㣬�������������ĵ���ֵ
*/
float PID_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Speed)
{   	
	Current_Speed = Current_Speed/PID->Speed_Ratio;//���ٱ�
	
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
	//����
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








































