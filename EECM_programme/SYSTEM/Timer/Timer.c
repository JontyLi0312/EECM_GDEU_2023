void TIM6_Init(void) 
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能TIM6时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // 配置TIM6为基本定时器，时钟源为内部时钟
    TIM_TimeBaseStructure.TIM_Prescaler = 8400-1; // 时钟预分频，得到10 kHz的计数频率
    TIM_TimeBaseStructure.TIM_Period = 2000-1;    // 自动重装载值，实现20 ms的定时
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // 使能TIM6更新中断
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动TIM6定时器
    TIM_Cmd(TIM6, ENABLE);
}

// TIM6中断服务函数
void TIM6_DAC_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 
    {
        // 在这里执行20ms定时器触发后的操作
        // 清除中断标志位
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}