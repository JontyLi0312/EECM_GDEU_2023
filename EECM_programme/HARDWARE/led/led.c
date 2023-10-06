/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-06 16:19
 * @LastEditTime: 2023-10-06 16:23
 * @Description: led显示颜色选择结果
 */
#include "led.h"

void led_init(void);
void set_red_led(uint8_t status);
void set_blue_led(uint8_t status);
void set_green_led(uint8_t status);

void led_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef colour_led_initStructure;
    colour_led_initStructure.GPIO_Mode = GPIO_Mode_OUT;
    colour_led_initStructure.GPIO_OType = GPIO_OType_PP;
    colour_led_initStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
    colour_led_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    colour_led_initStructure.GPIO_Speed = GPIO_Medium_Speed;

    GPIO_Init(GPIOB, &colour_led_initStructure);

    set_red_led(0);
    set_blue_led(0);
    set_green_led(0);
}

void set_red_led(uint8_t status)
{
    if (status == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    }
}

void set_blue_led(uint8_t status)
{
    if (status == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    }
}

void set_green_led(uint8_t status)
{
    if (status == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_2);
    }
}
