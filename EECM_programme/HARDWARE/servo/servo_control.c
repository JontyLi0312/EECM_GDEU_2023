/**
 * @file   servo_control.c
 * @author coolersf (3328151400@qq.com)
 * @brief 用于最后结合角度控制舵机
 * @date 2023-10-08
 */
#include "servo_control.h"
#include "servo_apply.h"

float   g_Vertical_Kp=0.9, g_Vertical_Ki=0.005, g_Vertical_D =-3; 
int16_t g_Vertical_PWM;
jy901s_angleData g_angleData;           //结构体名字

//函数声明
int16_t Vertical(float Pitch,float Med_target);
void Servo_Limit(void);
void Servo_accomplish(void);

//将检测到的角度以及期望角度传入
//返回值为s_Pwm_Out 
int16_t Vertical(float Pitch,float Med_target)
{
	static int16_t s_Pwm_Out , s_Angle_Err , s_Angle_S,             
                   s_Angle_Err_Lowout , s_Angle_Err_Lowout_last;  //last相当于上一次偏差
	float a = 0.7;                                                //滤波系数
	s_Angle_Err = Pitch - Med_target;
	//滤波公式 low_out = (1-a)*Ek + a*low_out_last
	s_Angle_Err_Lowout = (1-a)*s_Angle_Err + a*s_Angle_Err_Lowout_last;    //滤波过后相当于偏差
	s_Angle_S += s_Angle_Err_Lowout;
	s_Angle_S = s_Angle_S>120?120:(s_Angle_S<(-120)?(-120):s_Angle_S);   //积分
	s_Pwm_Out = g_Vertical_Kp*s_Angle_Err_Lowout + g_Vertical_Ki*s_Angle_S + g_Vertical_D*(s_Angle_Err_Lowout -s_Angle_Err_Lowout_last);
	s_Angle_Err_Lowout_last = s_Angle_Err_Lowout;                       //将偏差传递
	return s_Pwm_Out;
}

//限舵机的幅度
void Servo_Limit(void)
{
    if(g_Vertical_PWM>580)
    {g_Vertical_PWM=580;}
   
    if(g_Vertical_PWM<188)
    {g_Vertical_PWM=188;}
}

//结合角度实现舵机的输出
void Servo_accomplish(void)
{
	    jy901s_getData(&g_angleData);
        g_Vertical_PWM = Vertical(g_angleData.roll,0)*2.973 + 405;
	    Servo_Limit();
		Servo_SetAngle_Vertical(g_Vertical_PWM); 

}
		
		
			
	
	
	