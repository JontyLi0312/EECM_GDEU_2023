#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

void motor_init(void);
void motor1_speed(uint16_t speed);
void motor2_speed(uint16_t speed);
void motor3_speed(uint16_t speed);
void motor4_speed(uint16_t speed);
void motor1_control(u8 mode);
void motor2_control(u8 mode);
void motor3_control(u8 mode);
void motor4_control(u8 mode);

#endif