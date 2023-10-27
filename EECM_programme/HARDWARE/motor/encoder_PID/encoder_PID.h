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
extern PID ASR3;
extern PID ASR4;


extern int16_t output1;
extern int16_t output2;
extern int16_t output3;
extern int16_t output4;



void PID_Init(void);
void pid_calc(PID *p);
void PID_Move(int16_t speed, int8_t Dre);
void PID_apply(void);
uint16_t Num_Abs(int16_t Encoder);

#endif