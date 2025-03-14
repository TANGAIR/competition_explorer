#ifndef _CHASSIS_H
#define _CHASSIS_H
#include "sys.h"


//�ٶ�ת������500ת6020.
#define SPEED_RIOT 16

/*******************��������************************/



void Chassis_Control(int left_speed,int right_speed );
void PID_track(float pwm_left, float pwm_right);
void IMU_track(float pwm_left, float pwm_right);
void Stop(void);


extern int PID_LEVEL_1,
           PID_LEVEL_2,
           PID_LEVEL_3;
extern int PID_left,
           PID_right;


#endif

