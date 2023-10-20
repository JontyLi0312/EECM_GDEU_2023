#include "stm32f4xx.h" // Device header

void grayscale_init(void);
u8 grayScale_detect(void);

void grayscale_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_grayscale_initStructure;
    GPIO_grayscale_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_grayscale_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_grayscale_initStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 |
                                            GPIO_Pin_13 | GPIO_Pin_14 |
                                            GPIO_Pin_15;
    GPIO_grayscale_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_grayscale_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_grayscale_initStructure);
}

/**
 * @brief 获取方向
 *
 * @return u8 direction
 *          @arg 1 右
 *          @arg 2 左
 *          @arg 3 停
 *          @arg 0 前
 */
u8 grayScale_detect(void)
{
    u8 left, mid_left, right, mid_right, mid;
    u8 forward;
    u8 direction;

    left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    mid_left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
    right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    mid_right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    mid = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

    forward = mid_left || mid_right || mid;

    if (mid_left == 0)
    {
        direction = 1;
    }
    else if (mid_right == 0)
    {
        direction = 2;
    }
    else if (forward == 0)
    {
        direction = 3;
    }
    else
    {
        direction = 0;
    }

    return direction;
}
