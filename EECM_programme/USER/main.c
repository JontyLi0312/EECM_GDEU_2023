/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-08-28 18:47
 * @LastEditTime: 2023-09-14 17:57
 * @Description: EECM_main.c
 */

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "key.h"

int main(void)
{
    delay_init(168);
    OLED_Init();
    key_init();

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WAIT", 8, 0);
    OLED_Refresh();

    while (1)
    {
        // 等待开始
        uint16_t start_flag;
        start_flag = start_key_get();

        while (start_flag)
        {
            OLED_Clear();
            OLED_ShowString(0, 0, (unsigned char *)"Status: WORK", 8, 0);
            OLED_ShowString(0, 10, (unsigned char *)"Select Colour, PLZ", 8, 0);
            OLED_Refresh();

            // 等待颜色选择
            while (1)
            {
                uint8_t colour_flag;
                colour_flag = colour_key_get();
                if (colour_flag != 0)
                {
                    uint8_t colour;
                    if (colour_flag == 1)
                    {
                        colour = "red               ";
                    }
                    else if (colour_flag == 2)
                    {
                        colour = "blue              ";
                    }
                    else if (colour_flag == 3)
                    {
                        colour = "green             ";
                    }

                    OLED_ShowString(0, 10, colour, 8, 0);
                    OLED_Refresh();
                    delay_ms(1000);
                    break;
                }
            }
            break;
        }
    }
}
