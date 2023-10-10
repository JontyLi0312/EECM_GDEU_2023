#include "stm32f4xx.h" // Device header

void grayscale_init(void);

void grayscale_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitTypeDef GPIO_grayscale_initStructure;
    GPIO_grayscale_initStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_grayscale_initStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_grayscale_initStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                            GPIO_Pin_2 | GPIO_Pin_3 |
                                            GPIO_Pin_4;
    GPIO_grayscale_initStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_grayscale_initStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOF, &GPIO_grayscale_initStructure);
}
