#ifndef __ENCODER_PID_H
#define __ENCODER_PID_H
#include "sys.h"

typedef struct
{
    int16_t Ref;
    int16_t Fdb;

    float Kp;
    float Ki;
    float Kd;

    float T;

    float a0;
    float a1;
    float a2;

    float Err;
    float Err_1;
    float Err_2;

    float Out;
    float Out_1;
    float OutMax;
    float OutMin;

} PID;
extern PID ASR1;
extern PID ASR2;


extern int16_t output1;
extern int16_t output2;


extern int8_t n;
extern int8_t a;

void PID_Init(void);
void pid_calc(PID *p);
void PID_Move(int16_t speed, int8_t Dre);


#endif