/**
 * @file motor_PWM.c
 * @author Jonty (ljt20030312@Outlook.com)
 * @date 2023-10-08
 * @brief 减速电机PWM波初始化
 */
#include "motor_PWM.h"

void motor_PWM_init(void);

/**
 * @brief 减速电机PWM初始化
 *
 */
void motor_PWM_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_motor_PWM_initStructure;
    GPIO_motor_PWM_initStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_motor_PWM_initStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_motor_PWM_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_motor_PWM_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_motor_PWM_initStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOC, &GPIO_motor_PWM_initStructure);
    GPIO_motor_PWM_initStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_motor_PWM_initStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

    TIM_TimeBaseInitTypeDef TIM_motor_PWM_initSctucture;
    TIM_motor_PWM_initSctucture.TIM_Period = 1999;
    TIM_motor_PWM_initSctucture.TIM_Prescaler = 71;
    TIM_motor_PWM_initSctucture.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_motor_PWM_initSctucture.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_motor_PWM_initSctucture);

    TIM_OCInitTypeDef TIM_OC_motor_PWM_initStructure;
    TIM_OCStructInit(&TIM_OC_motor_PWM_initStructure);
    TIM_OC_motor_PWM_initStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OC_motor_PWM_initStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;
    TIM_OC_motor_PWM_initStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC_motor_PWM_initStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OC_motor_PWM_initStructure);
    TIM_OC2Init(TIM3, &TIM_OC_motor_PWM_initStructure);
    TIM_OC3Init(TIM3, &TIM_OC_motor_PWM_initStructure);
    TIM_OC4Init(TIM3, &TIM_OC_motor_PWM_initStructure);

    TIM_Cmd(TIM3, ENABLE);
}