/**
 * @file servo_apply.c
 * @author coolersf (3328151400@qq.com)
 * @brief 用于直接调用改变PWM以及初始化PWM的函数和最后实现倒珠子的功能
 * @date 2023-10-08
 */
#include "servo_PWM.h"
void Servo_PWM_Init(void)
{
	servo_PWM();
}

void Servo_SetAngle_Level(float Angle)       
{
	PWM_SetCompare_Level(Angle);
}
void Servo_SetAngle_Vertical(float Angle)
{
	PWM_SetCompare_Vertical(Angle);
}

void Servo_Action(void)
{
	PWM_SetCompare_Level(521);
    PWM_SetCompare_Vertical(110);
}
