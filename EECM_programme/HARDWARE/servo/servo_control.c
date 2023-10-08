#include "servo_control.h"
#include "servo_apply.h"
float   g_Vertical_Kp=0.9, g_Vertical_Ki=0.005, g_Vertical_D =-3; 
int16_t g_Vertical_PWM;
struct jy901s_angleData g_angleData;


int16_t Vertical(float Pitch,float Med_target);
void Servo_Limit(void);


int16_t Vertical(float Pitch,float Med_target)
{
	static int16_t s_Pwm_Out , s_Angle_Err , s_Angle_S,             
                   s_Angle_Err_Lowout , s_Angle_Err_Lowout_last;  //last相当于上一次偏差
	float a = 0.7;
	s_Angle_Err = Pitch - Med_target;
	//滤波公式 low_out = (1-a)*Ek + a*low_out_last
	s_Angle_Err_Lowout = (1-a)*s_Angle_Err + a*s_Angle_Err_Lowout_last;    //滤波过后相当于偏差
	s_Angle_S += s_Angle_Err_Lowout;
	s_Angle_S = s_Angle_S>120?120:(s_Angle_S<(-120)?(-120):s_Angle_S);
	s_Pwm_Out = g_Vertical_Kp*s_Angle_Err_Lowout + g_Vertical_Ki*s_Angle_S + g_Vertical_D*(s_Angle_Err_Lowout -s_Angle_Err_Lowout_last);
	s_Angle_Err_Lowout_last = s_Angle_Err_Lowout;
	return s_Pwm_Out;
}

//限幅度
void Servo_Limit(void)
{
    if(g_Vertical_PWM>580)
    {g_Vertical_PWM=580;}
   
    if(g_Vertical_PWM<110)
    {g_Vertical_PWM=110;}
}


void Servo_accomplish()
{
        g_Vertical_PWM = Vertical(g_angleData.Pitch,0)*2.973 + 345  ;
	    Servo_Limit();
		Servo_SetAngle_Vertical(g_Vertical_PWM); 

}
		
		
			
	
	
	