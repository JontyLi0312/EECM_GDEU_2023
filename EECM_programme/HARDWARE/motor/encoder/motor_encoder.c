#include "motor_encoder.h"

void motorEncoder_init(void);
void motor1Encoder_init(void);
void motor2Encoder_init(void);
void motor3Encoder_init(void);
void motor4Encoder_init(void);
int16_t Read_Speed(uint8_t motor);
GPIO_InitTypeDef g_GPIO_InitStructure;
TIM_TimeBaseInitTypeDef g_TIM_TimeBaseStructure;
TIM_ICInitTypeDef g_TIM_ICInitStructure;
NVIC_InitTypeDef g_NVIC_InitStructure;

/**
 * @brief 四个电机编码器初始化
 *
 */
void motorEncoder_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    motor1Encoder_init();
    motor2Encoder_init();
    motor3Encoder_init();
    motor4Encoder_init();
}

/**
 * @brief 电机1编码器初始化
 *
 */
void motor1Encoder_init(void)
{
    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // 光栅尺
    g_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    g_GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    g_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    g_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &g_GPIO_InitStructure);
    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOE, &g_GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    TIM_TimeBaseStructInit(&g_TIM_TimeBaseStructure);
    g_TIM_TimeBaseStructure.TIM_Prescaler = 0;
    g_TIM_TimeBaseStructure.TIM_Period = 65535;
    g_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    g_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &g_TIM_TimeBaseStructure);

    // 编码器模式1 – 根据TI1FP1的电平，计数器在TI2FP2的边沿向上/下计数。
    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising); // 编码器接口模式配置
    TIM_ICStructInit(&g_TIM_ICInitStructure);          // 默认值赋值
    g_TIM_ICInitStructure.TIM_ICFilter = 0x2;          // 滤波 0x0~0xF
    TIM_ICInit(TIM1, &g_TIM_ICInitStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);      // 清除标志位
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // 更新中断

    TIM_SetCounter(TIM1, 0); // 计数器清零
    TIM_Cmd(TIM1, ENABLE);

    g_NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    g_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    g_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    g_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&g_NVIC_InitStructure);
}

/**
 * @brief 电机2编码器初始化
 *
 */
void motor2Encoder_init(void)
{
    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 光栅尺
    GPIO_Init(GPIOC, &g_GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

    TIM_TimeBaseInit(TIM8, &g_TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI1,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising); // 编码器接口模式配置                                                          // 滤波 0x0~0xF
    TIM_ICInit(TIM8, &g_TIM_ICInitStructure);

    TIM_ClearFlag(TIM8, TIM_FLAG_Update);      // 清除标志位
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); // 更新中断

    TIM_SetCounter(TIM8, 0); // 计数器清零
    TIM_Cmd(TIM8, ENABLE);

    g_NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
    NVIC_Init(&g_NVIC_InitStructure);
}

/**
 * @brief 电机3编码器初始化
 *
 */
void motor3Encoder_init(void)
{
    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // 光栅尺
    GPIO_Init(GPIOA, &g_GPIO_InitStructure);

    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // 光栅尺
    GPIO_Init(GPIOB, &g_GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

    TIM_TimeBaseInit(TIM2, &g_TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising); // 编码器接口模式配置                                                         // 滤波 0x0~0xF
    TIM_ICInit(TIM2, &g_TIM_ICInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 清除标志位
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 更新中断

    TIM_SetCounter(TIM2, 0); // 计数器清零
    TIM_Cmd(TIM2, ENABLE);

    g_NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_Init(&g_NVIC_InitStructure);
}

/**
 * @brief 电机4编码器初始化
 *
 */
void motor4Encoder_init(void)
{
    g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 光栅尺
    GPIO_Init(GPIOB, &g_GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

    TIM_TimeBaseInit(TIM4, &g_TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising); // 编码器接口模式配置

    TIM_ICInit(TIM4, &g_TIM_ICInitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);      // 清除标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // 更新中断

    TIM_SetCounter(TIM4, 0); // 计数器清零
    TIM_Cmd(TIM4, ENABLE);

    g_NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_Init(&g_NVIC_InitStructure);
}

/**
 * @brief 获取电机速度
 *
 * @param TIMX
 *        @arg TIM4
 *        @arg TIM1
 *        @arg TIM8
 *        @arg TIM2
 * @return int16_t speed_Value 返回CCR值
 */
int16_t Read_Speed(uint8_t TIMX)
{
    int16_t Speed_Value;
    switch (TIMX)
    {
    case 5:
        Speed_Value = TIM_GetCounter(TIM4);
        TIM_SetCounter(TIM4, 0);
        break;
    case 1:
        Speed_Value = TIM_GetCounter(TIM1);
        TIM_SetCounter(TIM1, 0);
        break;
    case 8:
        Speed_Value = TIM_GetCounter(TIM8);
        TIM_SetCounter(TIM8, 0);
        break;
    case 2:
        Speed_Value = TIM_GetCounter(TIM2);
        TIM_SetCounter(TIM2, 0);
        break;
    default:
        Speed_Value = 0;
    }
    return Speed_Value;
}