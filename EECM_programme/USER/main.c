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
#include "key.h"
#include "motor.h"
#include "jy901s.h"
#include "5graysacle.h"
#include "servo_control.h"
#include "servo_apply.h"
#include "Timer.h"
#include "motor_encoder.h"
#include "encoder_PID.h"
#include "UART5.h"

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
    Uart5_init(115200);
    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WAIT", 8, 1);
    OLED_Refresh();

    while (1)
    {
        OLED_ShowChar(0, 10, Serial_RxPacket[0], 8, 1);
        OLED_Refresh();
    }
    while (1)
    {

        // 等待开始
        u8 start_flag;
        start_flag = start_key_get();
        while (start_flag)
        {
            OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
            OLED_Refresh();

            u8 direction;
            direction = grayScale_detect();
            if (direction == 0)
            {
                // stop
                OLED_ShowString(0, 20, (unsigned char *)"stop", 8, 1);
                OLED_Refresh();
                break;
            }
            else if (direction == 1)
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
}
