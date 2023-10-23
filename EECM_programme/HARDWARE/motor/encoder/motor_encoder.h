#ifndef __MOTOR_ENCODER_H
#define __MOTOR_ENCODER_H
#include "sys.h"

void motorEncoder_init(void);
int16_t Read_Speed(uint8_t TIMX);
int16_t Read_Speed(uint8_t motor);
#endif
