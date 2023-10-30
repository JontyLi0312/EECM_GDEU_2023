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
// #include "key.h"
#include "motor.h"
#include "jy901s.h"
#include "5graysacle.h"
#include "servo_control.h"
#include "servo_apply.h"
#include "Timer.h"
#include "encoder_PID.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    OLED_Init();
    // key_init();
    motor_init();
    jy901s_init();
    grayscale_init();
    Servo_PWM_Init();
    PID_Init();
    TIM6_Init();

    // motor1_control(0);
    // motor2_control(0);
    // motor3_control(0);
    // motor4_control(0);
    // PID_Move(0, 1);
    // PID_Move(0, 2);
    // PID_Move(0, 3);
    // PID_Move(0, 4);

    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WAIT", 8, 1);
    OLED_Refresh();

    u8 start_flag;
    // start_flag = start_key_get();
    //  test
    start_flag = 0;
    // while (start_flag)
    // {
    //     // start_flag = start_key_get();
    // }

    // test
    while (1)
    {
        motor2_control(1);
        PID_Move(40, 2);
        motor1_control(1);
        PID_Move(40, 1);
        motor3_control(1);
        PID_Move(40, 3);
        motor4_control(1);
        PID_Move(40, 4);

        delay_ms(2000);

        motor2_control(2);
        PID_Move(40, 2);
        motor1_control(2);
        PID_Move(40, 1);
        motor3_control(2);
        PID_Move(40, 3);
        motor4_control(2);
        PID_Move(40, 4);

        delay_ms(2000);

        motor2_control(0);
        motor1_control(0);
        motor3_control(0);
        motor4_control(0);

        delay_ms(2000);
    }

    while (1)
    {
        OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
        OLED_Refresh();

        u8 direction;
        direction = grayScale_detect();
        if (direction == 's')
        {
            // stop
            OLED_ShowString(0, 20, (unsigned char *)"stop", 8, 1);
            OLED_Refresh();
        }
        else if (direction == 'f')
        {
            // forward
            OLED_ShowString(0, 20, (unsigned char *)"forward", 8, 1);
            OLED_Refresh();
        }
        else if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left", 8, 1);
            OLED_Refresh();
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
            OLED_Refresh();
        }
        else
        {
            // error
            OLED_ShowString(0, 20, (unsigned char *)"error", 8, 1);
            OLED_Refresh();
            break;
        }
    }
}