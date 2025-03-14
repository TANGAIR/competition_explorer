#ifndef _MAIN_H
#define _MAIN_H

/*head file include*/
#include "sys.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "can.h"
#include "tim2.h"
#include "PID.h"
#include "chassis.h"
#include "imu.h"
#include "gray_sensor.h"
#include "spi.h"
#include "kalman.h"
#include "task.h"
#include "ultrasonic.h"
#include "steering.h"

/***************宏定义区*********************/

/*CAN data*/
//主板地址
#define CAN_201_204_ID  0X200
//底盘
#define CAN1_CHASSIS_ID_Left  0x201
#define CAN1_CHASSIS_ID_Right   0x203

#define PID_mode(a,b) PID_track_Flag=1;IMU_track_right=0;PID_left=a;PID_right=b;
#define IMU_mode(i,j,k) PID_track_Flag=0;IMU_track_left=i;IMU_track_right=j;IMU_direction_angle=k;




extern char PID_track_Flag;
extern short IMU_track_right;
extern short IMU_track_left;
extern short IMU_direction_angle;


#endif

