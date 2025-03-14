#ifndef __GRAY_SENSOR_H
#define __GRAY_SENSOR_H 			   
#include "sys.h"	

//前F_L2、F_L1、F_R1、F_R2
//后B_L2、B_L1、B_R1、B_R2
	
//定义F1-F6，B1-B5为IO口读取的数据

#define F_L2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)
#define F_L1   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_12)
#define F_R1   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_11)
#define F_R2   GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_10)

#define B_L2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)
#define B_L1   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)
#define B_R1   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)
#define B_R2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)

#define HongWai   GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_2)//没检测到返回1，检测到返回0
void gray_sensor_init(void);//灰度传感器初始化


#endif
