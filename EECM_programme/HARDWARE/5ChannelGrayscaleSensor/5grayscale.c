#include "5graysacle.h"
#include "delay.h"

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
 *          @arg 'L' turn left
 *          @arg 'R' turn right
 *          @arg 'S' stop
 *          @arg 'F' forward
 */
u8 grayScale_detect(void)
{
    u8 sensor_left, sensor_mid_left, sensor_right, sensor_mid_right, sensor_mid;
    u8 stop, backward, right, left, large_left, large_right;
    u8 direction;

    sensor_left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    sensor_right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
    sensor_mid = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    sensor_mid_left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
    sensor_mid_right = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);

    stop = sensor_mid_left || sensor_mid_right || sensor_mid || sensor_left || sensor_right;
    backward = sensor_mid_left || sensor_mid_right || sensor_mid_right;
    right = sensor_mid_right || !sensor_mid_left;
    left = sensor_mid_left || !sensor_mid_right;
    large_left = !sensor_left || sensor_mid_left || sensor_mid || sensor_mid_right || sensor_right;
    large_right = sensor_left || sensor_mid_left || sensor_mid || sensor_mid_right || !sensor_right;

    if (stop == 0)
    {
        direction = 'S';
    }
    else if (backward == 0)
    {
        direction = 'B';
    }
    else if (right == 0)
    {
        direction = 'L';
    }
    else if (left == 0)
    {
        direction = 'R';
    }
    else if (large_left == 0)
    {
        direction = 'L';
    }
    else if (large_right == 0)
    {
        direction = 'R';
    }
    else
    {
        direction = 'F';
    }

    return direction;
}
