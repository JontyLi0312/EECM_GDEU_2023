/**
 * @file servo_apply.c
 * @author coolersf (3328151400@qq.com)
 * @brief 用于直接调用改变PWM以及初始化PWM的函数和最后实现倒珠子的功能
 * @date 2023-10-08
 */
#ifndef __SERVO_APPLY_H
#define __SERVO_APPLY_H
#include "sys.h"
void Servo_Init(void);
void Servo_SetAngle_Level(float Angle);     
void Servo_SetAngle_Vertical(float Angle);
void Servo_Action(void);
void Servo_Reset(void);


#endif