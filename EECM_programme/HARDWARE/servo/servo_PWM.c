/**
 * @file servo_PWM.c
 * @author coolersf (3328151400@qq.com)
 * @date 2023-10-08
 * @Description: 用于控制舵机的PWM
 */
#include "servo_PWM.h"
void Servo_PWM (void) ;
void PWM_SetCompare_Level  (uint16_t Compare);
void PWM_SetCompare_Vertical(uint16_t Compare);

void Servo_PWM (void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    GPIO_InitTypeDef servo_PWM_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    
    servo_PWM_InitStructure.GPIO_Mode  =   GPIO_Mode_AF;
    servo_PWM_InitStructure.GPIO_Speed =  GPIO_High_Speed ;
    servo_PWM_InitStructure.GPIO_Pin   =    GPIO_Pin_6 | GPIO_Pin_7;
    servo_PWM_InitStructure.GPIO_OType =  GPIO_OType_PP;
    servo_PWM_InitStructure.GPIO_PuPd  =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &servo_PWM_InitStructure);
    
    
    TIM_TimeBaseInitTypeDef TIM4_PWM_initStructure;
	TIM4_PWM_initStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM4_PWM_initStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM4_PWM_initStructure.TIM_Period = 5000 - 1;		                  //ARR           
	TIM4_PWM_initStructure.TIM_Prescaler = 288 - 1;	           	    //PSC
	TIM4_PWM_initStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM4_PWM_initStructure);
	
	TIM_OCInitTypeDef TIM4_OC_initStructure;
	TIM_OCStructInit(&TIM4_OC_initStructure);                           //结构体赋初始值
	TIM4_OC_initStructure.TIM_OCMode = TIM_OCMode_PWM1;                 //输出比较模式： PWM1模式
	TIM4_OC_initStructure.TIM_OCPolarity = TIM_OCPolarity_High;         //极性选择
	TIM4_OC_initStructure.TIM_OutputState = TIM_OutputState_Enable;     //输出电路使能
	TIM4_OC_initStructure.TIM_Pulse = 0;		                        //CCR的值
	
	TIM_OC1Init(TIM4, &TIM4_OC_initStructure);
	TIM_OC2Init(TIM4, &TIM4_OC_initStructure);
	
	TIM_Cmd(TIM4, ENABLE);                                            //使能TIM4
}

void PWM_SetCompare_Level(uint16_t Compare)
{
	TIM_SetCompare1(TIM4, Compare);    //Compare PB6
}


void PWM_SetCompare_Vertical(uint16_t Compare)
{
	TIM_SetCompare2(TIM4, Compare);    //Compare PB7
}




