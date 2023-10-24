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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    GPIO_InitTypeDef servo_PWM_InitStructure;
    
    servo_PWM_InitStructure.GPIO_Mode  =   GPIO_Mode_AF;
    servo_PWM_InitStructure.GPIO_Speed =  GPIO_High_Speed ;
    servo_PWM_InitStructure.GPIO_Pin   =    GPIO_Pin_1 | GPIO_Pin_2;
    servo_PWM_InitStructure.GPIO_OType =  GPIO_OType_PP;
    servo_PWM_InitStructure.GPIO_PuPd  =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &servo_PWM_InitStructure);
    
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
    
    TIM_TimeBaseInitTypeDef TIM5_PWM_initStructure;
	TIM5_PWM_initStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM5_PWM_initStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM5_PWM_initStructure.TIM_Period = 5000 - 1;		                  //ARR           
	TIM5_PWM_initStructure.TIM_Prescaler = 288 - 1;	           	    //PSC
	TIM5_PWM_initStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM5_PWM_initStructure);
	
	TIM_OCInitTypeDef TIM5_OC_initStructure;
	TIM_OCStructInit(&TIM5_OC_initStructure);                           //结构体赋初始值
	TIM5_OC_initStructure.TIM_OCMode = TIM_OCMode_PWM1;                 //输出比较模式： PWM1模式
	TIM5_OC_initStructure.TIM_OCPolarity = TIM_OCPolarity_High;         //极性选择
	TIM5_OC_initStructure.TIM_OutputState = TIM_OutputState_Enable;     //输出电路使能
	TIM5_OC_initStructure.TIM_Pulse = 0;		                        //CCR的值
	
	TIM_OC2Init(TIM5, &TIM5_OC_initStructure);
	TIM_OC3Init(TIM5, &TIM5_OC_initStructure);
	
	TIM_Cmd(TIM5, ENABLE);                                            //使能TIM5
}

void PWM_SetCompare_Level(uint16_t Compare)
{
	TIM_SetCompare2(TIM5, Compare);    //Compare PB6
}


void PWM_SetCompare_Vertical(uint16_t Compare)
{
	TIM_SetCompare3(TIM5, Compare);    //Compare PB7
}




