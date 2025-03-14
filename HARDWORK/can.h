#ifndef __CAN_H
#define __CAN_H 			   
#include "sys.h"


/*******************结构体声明************************/
typedef struct
{
	short Real_Speed;             //电机反馈的值
	short Target_Speed;           //CAN发送的值
	
	
	
}CAN_Data_TypeDef;

/*******************函数声明************************/

void CAN1_Init(void);
void CAN1_TX(void);


/**************************数据作用域全局拓展******************************/


extern CAN_Data_TypeDef  
                  //底盘电机四个M3508
                  CAN_DATA_Left,  //201左前轮
								  CAN_DATA_LeftBack,   //202左后轮
									CAN_DATA_Right,  //203右后轮
								  CAN_DATA_RightFront; //204右前轮
        


#endif


