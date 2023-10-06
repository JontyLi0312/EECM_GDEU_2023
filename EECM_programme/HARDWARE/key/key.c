/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-04 17:29
 * @LastEditTime: 2023-10-06 15:32
 * @Description:
 */
/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-04 14:41
 * @LastEditTime: 2023-10-04 15:14
 * @Description: 开始按钮，颜色选择按钮
 */
#include "key.h"
#include "delay.h"

void key_init(void);
uint8_t colour_key_get(void);
uint16_t start_key_get(void);

/**
 * @brief 按键初始化
 *
 */
void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // 颜色选择按钮初始化
    GPIO_InitTypeDef GPIO_colourKey_initStructure;
    GPIO_colourKey_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_colourKey_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_colourKey_initStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_colourKey_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_colourKey_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOA, &GPIO_colourKey_initStructure);

    // 开始按钮
    GPIO_InitTypeDef GPIO_startKey_initStructure;
    GPIO_startKey_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_startKey_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_startKey_initStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_startKey_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_startKey_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOA, &GPIO_startKey_initStructure);
}

/**
 * @brief 获取按键
 *
 * @return uint8_t colour_key_flag
 *         @arg 1 red
 *         @arg 2 blue
 *         @arg 3 green
 *         @arg 4 none
 */
uint8_t colour_key_get(void)
{
    uint8_t colour_key_flag;

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
    {
        delay_ms(20);

        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
            ;

        delay_ms(20);

        colour_key_flag = 1;
    }
    else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
    {
        delay_ms(20);

        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)

            delay_ms(20);

        colour_key_flag = 2;
    }
    else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
    {
        delay_ms(20);

        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6 == 0))
            ;

        delay_ms(20);

        colour_key_flag = 3;
    }
    else
    {
        colour_key_flag = 0;
    }

    return colour_key_flag;
}

/**
 * @brief 开始按钮状态获取
 *
 * @return uint16_t
 *         @arg 1 开始
 *         @arg 0 等待
 */
uint16_t start_key_get(void)
{
    uint16_t start_key_flag;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
    {
        delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
            ;
        delay_ms(20);

        start_key_flag = 1;
    }
    else
        start_key_flag = 0;

    return start_key_flag;
}