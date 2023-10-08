#ifndef __MOTOR_PWM_H
#define __MOTOR_PWM_H
#include "sys.h"

void motor_PWM_init(void);
void motor1_speed_control(uint16_t speed);
void motor2_speed_control(uint16_t speed);
void motor3_speed_control(uint16_t speed);
void motor4_speed_control(uint16_t speed);

#endif
