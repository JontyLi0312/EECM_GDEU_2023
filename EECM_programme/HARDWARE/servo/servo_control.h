#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H
#include "sys.h"
struct jy901s_angleData {
    float Roll;
    float Pitch;
    float Yaw;
};
void Servo_Limit(void);
int16_t Vertical(float Pitch,float Med_target);
void Servo_accomplish();
#endif