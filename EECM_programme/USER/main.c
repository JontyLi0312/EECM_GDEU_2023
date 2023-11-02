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

    stop();

    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();

    forward();
    delay_ms(50);

    int i = 0;

    while (1)
    {
        u8 direction, flag;
        direction = grayScale_detect();
        if (direction != 'F')
        {
            delay_ms(5);
            i++;
            if (i == 3)
            {
                i = 0;
            }
        }
        else
        {
            i--;
        }

        if (i == 2)
        {
            if (direction == 'S')
            {
                // stop
                OLED_ShowString(0, 20, (unsigned char *)"stop      ", 8, 1);
                OLED_Refresh();

                backward();
                delay_ms(50);
            }
            else if (direction == 'l')
            {
                // turn left
                OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
                OLED_Refresh();

                turn_left();
            }
            else if (direction == 'r')
            {
                // turn right
                OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
                OLED_Refresh();

                turn_right();
            }
        }
        else
        {
            OLED_ShowString(0, 20, (unsigned char *)"forward   ", 8, 1);
            OLED_Refresh();

            forward();
        }
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
    PID_Move(45, 1);
    motor2_control(1);
    PID_Move(45, 2);
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
    PID_Move(45, 3);
    motor4_control(1);
    PID_Move(45, 4);
}

/**
 * @brief car forward
 *
 */
void forward(void)
{
    motor1_control(1);
    PID_Move(35, 1);
    motor2_control(1);
    PID_Move(35, 2);
    motor3_control(1);
    PID_Move(35, 3);
    motor4_control(1);
    PID_Move(35, 4);
}

/**
 * @brief car backward
 *
 */
void backward(void)
{
    motor1_control(2);
    PID_Move(30, 1);
    motor2_control(2);
    PID_Move(30, 2);
    motor3_control(2);
    PID_Move(30, 3);
    motor4_control(2);
    PID_Move(30, 4);
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
