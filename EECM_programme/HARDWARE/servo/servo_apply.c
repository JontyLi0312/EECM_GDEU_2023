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
