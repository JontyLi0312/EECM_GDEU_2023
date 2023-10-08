#ifndef __SERVO_PWM_H
#define __SERVO_PWM_H
#include "sys.h"
void servo_PWM_init (void);
void PWM_SetCompare_Level(uint16_t Compare);
void PWM_SetCompare_Vertical(uint16_t Compare);

#endif