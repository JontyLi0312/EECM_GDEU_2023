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

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    delay_init(168);
    OLED_Init();
    key_init();
    motor_init();

    delay_ms(500);

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WAIT", 8, 1);
    OLED_Refresh();

    while (1)
    {
        // 等待开始
        uint16_t start_flag;
        start_flag = start_key_get();

        while (start_flag)
        {
            OLED_Clear();
            OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
            OLED_ShowString(0, 10, (unsigned char *)"Select Colour, PLZ", 8, 1);
            OLED_Refresh();

            // 等待颜色选择
            while (1)
            {
                uint8_t colour_flag;
                colour_flag = colour_key_get();
                if (colour_flag != 0)
                {
                    if (colour_flag == 1)
                    {
                        OLED_ShowString(0, 10, (unsigned char *)"SELECT : RED      ", 8, 1);
                    }
                    else if (colour_flag == 2)
                    {
                        OLED_ShowString(0, 10, (unsigned char *)"SELECT : BLUE     ", 8, 1);
                    }
                    else if (colour_flag == 3)
                    {
                        OLED_ShowString(0, 10, (unsigned char *)"SELECT : GREEN    ", 8, 1);
                    }

                    OLED_Refresh();

                    break;
                }
            }
            break;
        }
    }
}

// todo 减速电机前进后退控制
