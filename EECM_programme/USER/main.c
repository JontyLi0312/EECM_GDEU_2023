/**
 * @file main.c
 * @author Jonty (ljt20030312@Outlook.com)
 * @date 2023-10-08
 * @brief EECM_main.c
 */
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "motor.h"
#include "jy901s.h"
#include "5graysacle.h"
#include "Timer.h"
#include "encoder_PID.h"

void turn_left(void);
void turn_right(void);
void forward(void);
void backward(void);
void stop(void);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    OLED_Init();
    motor_init();
    jy901s_init();
    grayscale_init();
    PID_Init();
    TIM6_Init();

    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();

    forward();
    delay_ms(100);

    while (1)
    {
        u8 direction, flag;
        direction = grayScale_detect();
        if (direction == 'S')
        {
            // stop
            OLED_ShowString(0, 20, (unsigned char *)"stop      ", 8, 1);
            OLED_Refresh();

            stop();
        }
        else if (direction == 'B')
        {
            // backward
            OLED_ShowString(0, 20, (unsigned char *)"backward  ", 8, 1);
            OLED_Refresh();

            backward();
        }
        else if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
            OLED_Refresh();

            turn_left();
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
            OLED_Refresh();

            turn_right();
        }
        else
        {
            OLED_ShowString(0, 20, (unsigned char *)"forward   ", 8, 1);
            OLED_Refresh();

            forward();
        }

        delay_ms(10);
    }

    return 0;
}

/**
 * @brief car turn left
 *
 */
void turn_left(void)
{
    motor1_control(1);
    PID_Move(65, 1);
    motor2_control(1);
    PID_Move(65, 2);
    motor3_control(1);
    PID_Move(5, 3);
    motor4_control(1);
    PID_Move(5, 4);
}

/**
 * @brief car turn right
 *
 */
void turn_right(void)
{
    motor1_control(1);
    PID_Move(5, 1);
    motor2_control(1);
    PID_Move(5, 2);
    motor3_control(1);
    PID_Move(65, 3);
    motor4_control(1);
    PID_Move(65, 4);
}

/**
 * @brief car forward
 *
 */
void forward(void)
{
    motor1_control(1);
    PID_Move(40, 1);
    motor2_control(1);
    PID_Move(40, 2);
    motor3_control(1);
    PID_Move(40, 3);
    motor4_control(1);
    PID_Move(40, 4);
}

/**
 * @brief car backward
 *
 */
void backward(void)
{
    motor1_control(2);
    PID_Move(15, 1);
    motor2_control(2);
    PID_Move(15, 2);
    motor3_control(2);
    PID_Move(15, 3);
    motor4_control(2);
    PID_Move(15, 4);
}

/**
 * @brief car stop
 *
 */
void stop(void)
{
    motor1_control(0);
    motor2_control(0);
    motor3_control(0);
    motor4_control(0);
}
