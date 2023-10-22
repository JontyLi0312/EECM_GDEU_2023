/**
 * @file motor.c
 * @author Jonty (ljt20030312@Outlook.com)
 * @date 2023-10-08
 * @brief 减速电机初始化及控制
 */
#include "motor.h"
#include "motor_PWM.h"
#include "motor_encoder.h"

void motor_init(void);
void motor1_speed(u16 speed);
void motor2_speed(u16 speed);
void motor3_speed(u16 speed);
void motor4_speed(u16 speed);
void motor1_control(u8 mode);
void motor2_control(u8 mode);
void motor3_control(u8 mode);
void motor4_control(u8 mode);

/**
 * @brief 减速电机控制引脚初始化
 *
 */
void motor_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_motor_initStructure;
    GPIO_motor_initStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                        GPIO_Pin_2 | GPIO_Pin_3 |
                                        GPIO_Pin_4 | GPIO_Pin_5 |
                                        GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_motor_initStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_motor_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_motor_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_motor_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOC, &GPIO_motor_initStructure);

    motor_PWM_init();
    motorEncoder_init();
}

/**
 * @brief 减速电机1调速
 *
 * @param speed 速度，speed值为0-100，代表0速到100%满速
 */
void motor1_speed(u16 speed)
{
    u16 CCR;
    CCR = speed * 20;

    TIM_SetCompare1(TIM3, CCR);
}

/**
 * @brief 减速电机2调速
 *
 * @param speed 速度，speed值为0-100，代表0速到100%满速
 */
void motor2_speed(u16 speed)
{
    u16 CCR;
    CCR = speed * 20;

    TIM_SetCompare2(TIM3, CCR);
}

/**
 * @brief 减速电机3调速
 *
 * @param speed 速度，speed值为0-100，代表0速到100%满速
 */
void motor3_speed(u16 speed)
{
    u16 CCR;
    CCR = speed * 20;

    TIM_SetCompare3(TIM3, CCR);
}

/**
 * @brief 减速电机4调速
 *
 * @param speed 速度，speed值为0-100，代表0速到100%满速
 */
void motor4_speed(u16 speed)
{
    u16 CCR;
    CCR = speed * 20;

    TIM_SetCompare4(TIM3, CCR);
}

/**
 * @brief 减速电机1模式控制
 *
 * @param mode
 *        @arg 1 forward
 *        @arg 2 backward
 *        @arg 0 stop
 */
void motor1_control(u8 mode)
{
    if (mode == 1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_0);
        GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    }
    else if (mode == 2)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_0);
        GPIO_SetBits(GPIOC, GPIO_Pin_0);
    }
    else if (mode == 0)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_0);
        GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    }
}

/**
 * @brief 减速电机2模式控制
 *
 * @param mode
 *        @arg 1 forward
 *        @arg 2 backward
 *        @arg 0 stop
 */
void motor2_control(u8 mode)
{
    if (mode == 1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_2);
        GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    }
    else if (mode == 2)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_2);
        GPIO_SetBits(GPIOC, GPIO_Pin_3);
    }
    else if (mode == 0)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_2);
        GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    }
}

/**
 * @brief 减速电机3模式控制
 *
 * @param mode
 *        @arg 1 forward
 *        @arg 2 backward
 *        @arg 0 stop
 */
void motor3_control(u8 mode)
{
    if (mode == 1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_4);
        GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    }
    else if (mode == 2)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_4);
        GPIO_SetBits(GPIOC, GPIO_Pin_5);
    }
    else if (mode == 0)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_4);
        GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    }
}

/**
 * @brief 减速电机4模式控制
 *
 * @param mode
 *        @arg 1 forward
 *        @arg 2 backward
 *        @arg 0 stop
 */
void motor4_control(u8 mode)
{
    if (mode == 1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_10);
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);
    }
    else if (mode == 2)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_10);
        GPIO_SetBits(GPIOC, GPIO_Pin_11);
    }
    else if (mode == 0)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_10);
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);
    }
}
