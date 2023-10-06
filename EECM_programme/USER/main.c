/*** 
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-06 16:35
 * @LastEditTime: 2023-10-06 16:36
 * @Description: EECM_main.c
 */

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "key.h"
#include "led.h"

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
            OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 0);
            OLED_ShowString(0, 10, (unsigned char *)"Select Colour, PLZ", 8, 0);
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
                        set_red_led(1);
                    }
                    else if (colour_flag == 2)
                    {
                        set_blue_led(1);
                    }
                    else if (colour_flag == 3)
                    {
                        set_green_led(1);
                    }

                    OLED_ShowString(0, 10, (unsigned char *)"Select Finish!    ", 8, 0);
                    OLED_Refresh();
                    delay_ms(1000);
                    break;
                }
            }
            break;
        }
    }
}
