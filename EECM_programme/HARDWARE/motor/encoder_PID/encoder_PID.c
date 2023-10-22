#include "encoder_PID.h"
#include "motor_encoder.h"
#include "motor.h"

int16_t g_MOTO5,g_MOTO1,g_MOTO8,g_MOTO2;
int16_t g_PWM_MAX,g_PWM_MIN;
encoder_PID g_Speed_PID;

g_Speed_PID.kp = 0; 
g_Speed_PID.ki = 0;
g_Speed_PID.kd = 0;

int16_t Motor_Speed(int16_t Target_Speed,int16_t Encoder);

int16_t Motor_Speed(int16_t Target_Speed,int16_t Encoder)
{
    static int16_t s_Pwm_Out , s_Angle_Err , s_Angle_S,
    s_Angle_Err_Lowout , s_Angle_Err_Lowout_last;//last相当于上一次偏差
    float a = 0.7;                              //滤波系数
    s_Angle_Err = Encoder - Target_Speed;
    //滤波公式 low_out = (1-a)*Ek + a*low_out_last
   s_Angle_Err_Lowout = (1-a)*s_Angle_Err + a*s_Angle_Err_Lowout_last; //滤波过后相当于偏差
   s_Angle_S += s_Angle_Err_Lowout;
   s_Angle_S = s_Angle_S>120?120:(s_Angle_S<(-120)?(-120):s_Angle_S);    //积分
   s_Pwm_Out = g_Speed_PID.kp*s_Angle_Err_Lowout + g_Speed_PID.ki*s_Angle_S + g_Speed_PID.kd*(s_Angle_Err_Lowout -s_Angle_Err_Lowout_last);
   s_Angle_Err_Lowout_last = s_Angle_Err_Lowout;               //将偏差传递
   return s_Pwm_Out;
}


void Limit_Motor(int16_t *Pwm_Out)
{
    if(*Pwm_Out>g_PWM_MAX){*Pwm_Out = 7200;}
    if(*Pwm_Out<g_PWM_MIN){*Pwm_Out = -7200;}

}

void Motor_Load5(int16_t Target_Speed)
{
    int16_t g_Get_Encoder5 = Read_Speed(5);
    int16_t Pwm_Out =Motor_Speed(Target_Speed,g_Get_Encoder5);
    Limit_Motor(&Pwm_Out);
    motor1_speed(Pwm_Out);

}
void Motor_Load1(int16_t Target_Speed)
{
    int16_t g_Get_Encoder1 = Read_Speed(1);
    int16_t Pwm_Out = Motor_Speed(Target_Speed,g_Get_Encoder1);
    Limit_Motor(&Pwm_Out);
    motor2_speed(Pwm_Out);

}
void Motor_Load8(int16_t Target_Speed)
{
    int16_t g_Get_Encoder8 = Read_Speed(8);
    int16_t Pwm_Out = Motor_Speed(Target_Speed,g_Get_Encoder8);
    Limit_Motor(&Pwm_Out);
    motor3_speed(Pwm_Out);

}
void Motor_Load2(int16_t Target_Speed)
{
    int16_t g_Get_Encoder2 = Read_Speed(2);
    int16_t Pwm_Out = Motor_Speed(Target_Speed,g_Get_Encoder2);
    Limit_Motor(&Pwm_Out);
}