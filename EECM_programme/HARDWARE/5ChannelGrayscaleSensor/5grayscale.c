#include "stm32f4xx.h" // Device header

void grayscale_init(void);
u8 grayScale_detect(void);

void grayscale_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef GPIO_grayscale_initStructure;
    GPIO_grayscale_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_grayscale_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_grayscale_initStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 |
                                            GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_grayscale_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_grayscale_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOB, &GPIO_grayscale_initStructure);

    GPIO_grayscale_initStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOE, &GPIO_grayscale_initStructure);
}

/**
 * @brief 获取方向
 *
 * @return u8 direction
 *          @arg 'R' turn right
 *          @arg 'L' turn left
 *          @arg 'S' stop
 *          @arg 'F' forward
 */
u8 grayScale_detect(void)
{
    u8 left, mid_left, right, mid_right, mid;
    u8 stop;
    u8 direction;

    left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    mid = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
    mid_left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
    mid_right = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);

    stop = mid_left || mid_right || mid;
    right = !mid_left || !mid || mid_right;
    left = !mid_right || !mid || mid_left;

    if (stop == 0)
    {
        direction = 'S';
    }
    else if (right == 0)
    {
        direction = 'L';
    }
    else if (left == 0)
    {
        direction = 'R';
    }
    else
    {
        direction = 'F';
    }

    return direction;
}
