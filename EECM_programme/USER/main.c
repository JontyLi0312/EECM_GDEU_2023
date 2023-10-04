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

    // 开始
    uint16_t start_flag;
    start_flag = start_key_get();

    while (start_flag)
    {
        // 颜色选择
        while (1)
        {
            uint8_t colour;
            colour = colour_key_get();
            if (colour != 0)
            {
                OLED_ShowString(0, 0, (unsigned char *)"Finish Select", 8, 0);
                OLED_Refresh();
                delay_ms(1000);
                break;
            }
            else
            {
                OLED_ShowString(0, 0, (unsigned char *)"Select Colour, PLZ", 8, 0);
                OLED_Refresh();
            }
        }
    }
}
