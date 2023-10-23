/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-04 14:41
 * @LastEditTime: 2023-10-04 15:14
 * @Description: 开始按钮，颜色选择按钮
 */
#include "key.h"
#include "delay.h"

void key_init(void);
u8 start_key_get(void);

/**
 * @brief 开始按键初始化
 *
 */
void key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_startKey_initStructure;
    GPIO_startKey_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_startKey_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_startKey_initStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_startKey_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_startKey_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOA, &GPIO_startKey_initStructure);
}

/**
 * @brief 开始按钮状态获取
 *
 * @return uint16_t
 *         @arg 1 开始
 *         @arg 0 等待
 */
u8 start_key_get(void)
{
    u8 start_key_flag;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
    {
        delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
            ;
        delay_ms(20);

        start_key_flag = 1;
    }
    else
    {
        start_key_flag = 0;
    }

    return start_key_flag;
}
