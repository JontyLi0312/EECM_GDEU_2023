#include "encoder_PID.h"
#include "motor_encoder.h"
#include "motor.h"

void PID_Init();
void pid_calc(PID *p);
void PID_Move(int16_t speed, int8_t Dre);
void PID_apply();
uint16_t Num_Abs(int16_t Encoder);

PID ASR1;
PID ASR2;
PID ASR3;
PID ASR4;

void PID_Init()
{
    ASR1.Kp = 1; //
    ASR1.Ki = 0.005;
    ASR1.Kd = 0.001;
    ASR1.T = 0.02; //           20ms
    ASR1.OutMax = 90;
    ASR1.OutMin = 20;

    ASR2.Kp = 1; //
    ASR2.Ki = 0.005;
    ASR2.Kd = 0.0014;
    ASR2.T = 0.02; //           20ms
    ASR2.OutMax = 90;
    ASR2.OutMin = 20;
    ASR2.Kp = 1; //

    ASR3.Ki = 0.005;
    ASR3.Kd = 0.001;
    ASR3.T = 0.02; //           20ms
    ASR3.OutMax = 90;
    ASR3.OutMin = 20;

    ASR4.Kp = 1; //
    ASR4.Ki = 0.005;
    ASR4.Kd = 0.001;
    ASR4.T = 0.02; //           20ms
    ASR4.OutMax = 90;
    ASR4.OutMin = 20;
}

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
// 给定对于编码器的速度值以及电机序号
void PID_Move(int16_t speed, int8_t Dre)
{
    if (Dre == 1) //
    {
        ASR1.Ref = speed;
    }
    if (Dre == 2) //
    {

        ASR2.Ref = speed;
    }
    if (Dre == 3) //
    {
        ASR3.Ref = speed;
    }
    if (Dre == 4) //
    {

        ASR4.Ref = speed;
    }
}
//
void PID_apply()
{
    int16_t PWM1, PWM2,PWM3, PWM4;

    PWM1 = (int16_t)output1;
    PWM2 = (int16_t)output2;
    PWM3 = (int16_t)output3;
    PWM4 = (int16_t)output4;


    motor1_speed(PWM1);
    motor2_speed(PWM2);
    motor1_speed(PWM3);
    motor2_speed(PWM4);
}
// 绝对值函数，输入的是读取到编码器的值。使编码器的值变成正数，
uint16_t Num_Abs(int16_t Encoder)
{
    uint16_t Middle;
    Middle = Encoder > 0 ? Encoder : (-Encoder);
    return Middle;
}

// TIM6中断服务函数
void TIM6_DAC_IRQHandler(void)

{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // 在这里执行20ms定时器触发后的操作
    {
        // 让编码器的值变成整数
        ASR1.Fdb = Num_Abs(Read_Speed(1));
        ASR2.Fdb = Num_Abs(Read_Speed(2));
        ASR3.Fdb = Num_Abs(Read_Speed(3));
        ASR4.Fdb = Num_Abs(Read_Speed(4));


        ASR1.Err = ASR1.Ref - ASR1.Fdb;
        ASR2.Err = ASR2.Ref - ASR2.Fdb;
        ASR3.Err = ASR3.Ref - ASR3.Fdb;
        ASR4.Err = ASR4.Ref - ASR4.Fdb;

        pid_calc(&ASR1);
        pid_calc(&ASR2);
        pid_calc(&ASR3);
        pid_calc(&ASR4);

        output1 = ASR1.Out;
        output2 = ASR2.Out;
        output3 = ASR3.Out;
        output4 = ASR4.Out;

        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除中断标志位
    }
}
