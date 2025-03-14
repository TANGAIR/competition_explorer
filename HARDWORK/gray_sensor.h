#ifndef __GRAY_SENSOR_H
#define __GRAY_SENSOR_H 			   
#include "sys.h"	

//ǰF_L2��F_L1��F_R1��F_R2
//��B_L2��B_L1��B_R1��B_R2
	
//����F1-F6��B1-B5ΪIO�ڶ�ȡ������

#define F_L2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)
#define F_L1   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_12)
#define F_R1   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_11)
#define F_R2   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_10)

#define B_L2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)
#define B_L1   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)
#define B_R1   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)
#define B_R2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)

#define HongWai   GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_2)//û��⵽����1����⵽����0
void gray_sensor_init(void);//�Ҷȴ�������ʼ��


#endif
