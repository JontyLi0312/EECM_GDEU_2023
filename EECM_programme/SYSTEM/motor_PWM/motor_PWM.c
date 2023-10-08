#include "motor_PWM.h"

void motor_PWM_init(void);
void motor1_speed_control(uint16_t speed);
void motor2_speed_control(uint16_t speed);
void motor3_speed_control(uint16_t speed);
void motor4_speed_control(uint16_t speed);

void motor_PWM_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_motor_PWM_initStructure;
    GPIO_motor_PWM_initStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |
                                            GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_motor_PWM_initStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_motor_PWM_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_motor_PWM_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_motor_PWM_initStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(GPIOC, &GPIO_motor_PWM_initStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);

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

    TIM_Cmd(TIM3, ENABLE);
}

void motor1_speed_control(uint16_t speed)
{
    TIM_SetCompare1(TIM3, speed);
}

void motor2_speed_control(uint16_t speed)
{
    TIM_SetCompare2(TIM3, speed);
}

void motor3_speed_control(uint16_t speed)
{
    TIM_SetCompare3(TIM3, speed);
}

void motor4_speed_control(uint16_t speed)
{
    TIM_SetCompare4(TIM3, speed);
}