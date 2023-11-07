#include "encoder_PID.h"
#include "motor_encoder.h"
#include "motor.h"
#include "oled.h"
void PID_Init(void);
void pid_calc(PID *p);
void PID_Move(int16_t speed, int8_t Dre);
void PID_apply(void);
uint16_t Num_Abs(int16_t Encoder);

PID ASR1;
PID ASR2;
PID ASR3;
PID ASR4;
int16_t output1;
int16_t output2;
int16_t output3;
int16_t output4;

/**
 * @brief PID算法初始化
 *
 */
void PID_Init(void)
{
    ASR1.Kp = 6.8;
    ASR1.Ki = 55.81;
    ASR1.Kd = 0.0049;
    ASR1.T = 0.02; //           20ms
    ASR1.OutMax = 90;
    ASR1.OutMin = 0;

    ASR2.Kp = 6.8;
    ASR2.Ki = 55.81;
    ASR2.Kd = 0.0049;
    ASR2.T = 0.02; //           20ms
    ASR2.OutMax = 90;
    ASR2.OutMin = 0;

    ASR3.Kp = 6.8;
    ASR3.Ki = 55.81;
    ASR3.Kd = 0.0049;
    ASR3.T = 0.02; //           20ms
    ASR3.OutMax = 90;
    ASR3.OutMin = 0;

    ASR4.Kp = 6.8;
    ASR4.Ki = 55.81;
    ASR4.Kd = 0.0049;
    ASR4.T = 0.02; //           20ms
    ASR4.OutMax = 90;
    ASR4.OutMin = 0;
}

/**
 * @brief PID算法计算
 *
 * @param p
 */
void pid_calc(PID *p) //
{
    float a0, a1, a2;

    a0 = p->Kp + p->Ki * p->T + p->Kd / p->T;
    a1 = p->Kp + 2 * p->Kd / p->T;
    a2 = p->Kd / p->T;

    p->Out = p->Out_1 + a0 * p->Err - a1 * p->Err_1 + a2 * p->Err_2;

    if (p->Out > p->OutMax) // 限幅
        p->Out = p->OutMax;

    if (p->Out < p->OutMin)
        p->Out = p->OutMin;

    p->Out_1 = p->Out;
    p->Err_2 = p->Err_1;
    p->Err_1 = p->Err;
}

/**
 * @brief PID控速
 *
 * @param speed 速度值（1 - 100）百分比
 * @param Dre 需要控制的电机序号
 */
void PID_Move(int16_t speed, int8_t Dre)
{
    if (Dre == 1) //
    {
        ASR1.Ref = speed / 4.16;
    }
    if (Dre == 2) //
    {

        ASR2.Ref = speed / 4.16;
    }
    if (Dre == 3) //
    {
        ASR3.Ref = speed / 4.16;
    }
    if (Dre == 4) //
    {

        ASR4.Ref = speed / 4.16;
    }
}

/**
 * @brief PWM输入
 *
 */
void PID_apply(void)
{
    int16_t PWM1, PWM2, PWM3, PWM4;

    PWM1 = (int16_t)output1;
    PWM2 = (int16_t)output2;
    PWM3 = (int16_t)output3;
    PWM4 = (int16_t)output4;

    motor1_speed(PWM1);
    motor2_speed(PWM2);
    motor3_speed(PWM3);
    motor4_speed(PWM4);
}

/**
 * @brief 求绝对值
 *
 * @param Encoder 被绝对值的数
 * @return uint16_t
 */
uint16_t Num_Abs(int16_t Encoder)
{
    uint16_t Middle;
    Middle = Encoder > 0 ? Encoder : (-Encoder);
    return Middle;
}
