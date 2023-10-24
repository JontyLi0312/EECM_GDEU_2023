#include "encoder_PID.h"
#include "motor_encoder.h"
#include "motor.h"


void PID_Init();
void pid_calc(PID *p);
void PID_Move(int16_t speed, int8_t Dre);
void PID_apply();
PID ASR1;
PID ASR2;


void PID_Init()
{
    ASR1.Kp = 1; //        
    ASR1.Ki = 0.005;
    ASR1.Kd = 0.001;
    ASR1.T = 0.02; //           20ms
    ASR1.OutMax = +1000;
    ASR1.OutMin = -1000;

    ASR2.Kp = 1; //       
    ASR2.Ki = 0.005;
    ASR2.Kd = 0.001;
    ASR2.T = 0.02; //           20ms
    ASR2.OutMax = +1000;
    ASR2.OutMin = -1000;

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
//给定对于编码器的速度值以及电机序号
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
  
}

void PID_apply()
{
    int16_t PWMA, PWMB;

    PWMA = (int16_t)output1;
    PWMB = (int16_t)output2;
    
    if (PWMA < 20)
    {
        PWMA = 20;
    }
    else
    {
        if (PWMA > 90)
        {
            PWMA = 90;
        }
    }
    if (PWMB < 20)
    {
        PWMB = 20;
    }
    else
    {
        if (PWMB > 90)
        {
            PWMB = 90;
        }
    }
   
    motor1_speed(PWMA);
    motor1_speed(PWMB);
    
}

/******中断函数********/

void TIM4_IRQHandler(void)

{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {

        ASR1.Fdb = Read_Speed(1) * 10;
        ASR2.Fdb = Read_Speed(2) * 10;

        ASR1.Err = ASR1.Ref - ASR1.Fdb;
        ASR2.Err = ASR2.Ref - ASR2.Fdb;
      
        pid_calc(&ASR1);
        pid_calc(&ASR2);
        
        output1 = ASR1.Out;
        output2 = ASR2.Out;
       
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
