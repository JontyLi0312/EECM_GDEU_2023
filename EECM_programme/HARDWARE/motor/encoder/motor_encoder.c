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
    // g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // 光栅尺
    // g_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    // g_GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    // g_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // g_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    // GPIO_Init(GPIOA, &g_GPIO_InitStructure);

    // g_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    // GPIO_Init(GPIOE, &g_GPIO_InitStructure);

    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    // GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    // g_TIM_TimeBaseStructure.TIM_Prescaler = 0;
    // g_TIM_TimeBaseStructure.TIM_Period = 65535;
    // g_TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // g_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // g_TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // TIM_TimeBaseInit(TIM1, &g_TIM_TimeBaseStructure);

    // // 编码器模式1 – 根据TI1FP1的电平，计数器在TI2FP2的边沿向上/下计数。
    // TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1,
    //                            TIM_ICPolarity_Rising,
    //                            TIM_ICPolarity_Rising); // 编码器接口模式配置

    // TIM_ICStructInit(&g_TIM_ICInitStructure); // 默认值赋值
    // TIM_ICInit(TIM1, &g_TIM_ICInitStructure);

    // TIM_ClearFlag(TIM1, TIM_FLAG_Update); // 清除标志位

    // TIM_SetCounter(TIM1, 0); // 计数器清零
    // TIM_Cmd(TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // 使能定时器2的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能PA端口时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 使能PA端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;    // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 浮空输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 根据设定参数初始化GPIOA

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; // 浮空输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // 根据设定参数初始化GPIOA

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                    // 预分频器，不分频
    TIM_TimeBaseStructure.TIM_Period = 65535;                   // 设定计数器自动重装值，是为编码器转整圈时候的脉冲个数102000(0x)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式3
    TIM1->SR &= 0 << 0;
    TIM1->SR &= ~(1 << 0);
    TIM1->SR &= ~(1 << 3);
    TIM1->SR &= ~(1 << 4); // 清零处理SR寄存器的上述几位，否则在未发生编码器脉冲时也会进入中断

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); // 允许TIM1溢出中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Reset counter
    TIM_SetCounter(TIM1, 0);
    TIM_Cmd(TIM1, DISABLE);
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

    TIM_ClearFlag(TIM8, TIM_FLAG_Update); // 清除标志位

    TIM_SetCounter(TIM8, 0); // 计数器清零
    TIM_Cmd(TIM8, ENABLE);
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

    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除标志位

    TIM_SetCounter(TIM2, 0); // 计数器清零
    TIM_Cmd(TIM2, ENABLE);
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

    TIM_ClearFlag(TIM4, TIM_FLAG_Update); // 清除标志位

    TIM_SetCounter(TIM4, 0); // 计数器清零
    TIM_Cmd(TIM4, ENABLE);
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

int16_t Read_Speed(uint8_t motor)
{
    int16_t Speed_Value;
    switch (motor)
    {
    case 1:
        Speed_Value = TIM_GetCounter(TIM1);
        TIM_SetCounter(TIM1, 0);
        break;
    case 2:
        Speed_Value = TIM_GetCounter(TIM8);
        TIM_SetCounter(TIM8, 0);
        break;
    case 3:
        Speed_Value = TIM_GetCounter(TIM2);
        TIM_SetCounter(TIM2, 0);
        break;
    case 4:
        Speed_Value = TIM_GetCounter(TIM4);
        TIM_SetCounter(TIM4, 0);
        break;
    default:
        Speed_Value = 0;
    }
    return Speed_Value;
}