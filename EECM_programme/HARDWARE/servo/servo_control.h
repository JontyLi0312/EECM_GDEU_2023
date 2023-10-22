/**
 * @file   servo_control.c
 * @author coolersf (3328151400@qq.com)
 * @brief 用于最后结合角度控制舵机
 * @date 2023-10-08
 */
#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H
#include "sys.h"

void Servo_Limit(void);
int16_t Vertical(float Pitch,float Med_target);
void Servo_accomplish(void);
#endif