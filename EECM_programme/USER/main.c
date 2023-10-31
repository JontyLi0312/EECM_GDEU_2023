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

void turn_left(u16 time);
void turn_right(u16 time);
void forward(u16 time);
void backward(u16 time);
void stop(u16 time);

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

    stop(1);

    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();

    while (1)
    {
        u8 direction;
        direction = grayScale_detect();
        if (direction == 'S')
        {
            // stop
            OLED_ShowString(0, 20, (unsigned char *)"stop      ", 8, 1);
            OLED_Refresh();

            // stop(10);
        }
        else if (direction == 'F')
        {
            // forward
            OLED_ShowString(0, 20, (unsigned char *)"forward   ", 8, 1);
            OLED_Refresh();

            // forward(10);
        }
        else if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
            OLED_Refresh();

            // turn_left(10);
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn ritht", 8, 1);
            OLED_Refresh();

            // turn_right(10);
        }
        else
        {
            // error
            OLED_ShowString(0, 20, (unsigned char *)"error     ", 8, 1);
            OLED_Refresh();
            break;
        }
    }

    return 0;
}

/**
 * @brief car turn left
 *
 */
void turn_left(u16 time)
{
    motor1_control(1);
    PID_Move(30, 1);
    motor2_control(1);
    PID_Move(30, 2);
    motor3_control(1);
    PID_Move(15, 3);
    motor4_control(1);
    PID_Move(15, 4);
}

/**
 * @brief car turn right
 *
 */
void turn_right(u16 time)
{
    motor1_control(1);
    PID_Move(15, 1);
    motor2_control(1);
    PID_Move(15, 2);
    motor3_control(1);
    PID_Move(30, 3);
    motor4_control(1);
    PID_Move(30, 4);

    delay_ms(time);
}

/**
 * @brief car forward
 *
 */
void forward(u16 time)
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
void backward(u16 time)
{
    motor1_control(2);
    PID_Move(20, 1);
    motor2_control(2);
    PID_Move(20, 2);
    motor3_control(2);
    PID_Move(20, 3);
    motor4_control(2);
    PID_Move(20, 4);
}

/**
 * @brief car stop
 *
 */
void stop(u16 time)
{
    motor1_control(0);
    PID_Move(0, 1);
    motor2_control(0);
    PID_Move(0, 2);
    motor3_control(0);
    PID_Move(0, 3);
    motor4_control(0);
    PID_Move(0, 4);
}
