#ifndef __CAN_H
#define __CAN_H 			   
#include "sys.h"


/*******************�ṹ������************************/
typedef struct
{
	short Real_Speed;             //���������ֵ
	short Target_Speed;           //CAN���͵�ֵ
	
	
	
}CAN_Data_TypeDef;

/*******************��������************************/

void CAN1_Init(void);
void CAN1_TX(void);


/**************************����������ȫ����չ******************************/


extern CAN_Data_TypeDef  
                  //���̵���ĸ�M3508
                  CAN_DATA_Left,  //201��ǰ��
								  CAN_DATA_LeftBack,   //202�����
									CAN_DATA_Right,  //203�Һ���
								  CAN_DATA_RightFront; //204��ǰ��
        


#endif


