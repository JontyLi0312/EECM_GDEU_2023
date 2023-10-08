#ifndef __SERVO_APPLY_H
#define __SERVO_APPLY_H
#include "sys.h"
void Servo_PWM_Init(void);
void Servo_SetAngle_Level(float Angle);     
void Servo_SetAngle_Vertical(float Angle);
void Servo_Action(void);


#endif