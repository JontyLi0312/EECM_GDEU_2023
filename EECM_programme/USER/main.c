/**
 * @file main.c
 * @author Jonty (ljt20030312@Outlook.com)
 * @date 2023-10-08
 * @brief EECM_main.c
 */
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "key.h"
#include "motor.h"
#include "jy901s.h"
#include "5graysacle.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    delay_init(168);
    OLED_Init();
    key_init();
    motor_init();
    jy901s_init();
    grayscale_init();

    jy901s_angleData g_angleDatas;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WAIT", 8, 1);
    OLED_Refresh();

    while (1)
    {
        // 等待开始
        u8 start_flag;
        start_flag = start_key_get();

        while (start_flag)
        {
            OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
            OLED_Refresh();

            break;
        }
    }
}
