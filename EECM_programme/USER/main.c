/**
 * @file main.c
 * @author Jonty (ljt20030312@Outlook.com)
 * @date 2023-10-08
 * @brief EECM_main.c
 */

#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "motor.h"
#include "jy901s.h"
#include "5graysacle.h"
#include "Timer.h"
#include "encoder_PID.h"
#include "UART5.h"
#include "servo_apply.h"
#include "drive.h"
#include "motor_encoder.h"

u8 g_Serial_RxPacket[5] = {'0', '0', '0', '0', '0'};

/**
 * @brief 小车姿态数据
 *
 */
jy901s_angleData g_angleDatas;
/**
 * @brief 小车姿态基准值
 *
 */
float g_base_roll;
float g_base_pitch;
float g_base_yaw;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    OLED_Init();
    motor_init();
    jy901s_init();
    grayscale_init();
    PID_Init();
    Uart5_init();
    TIM6_Init();
    Servo_Init();

    jy901s_angleData g_angleDatas;

    jy901s_getData(&g_angleDatas);
    g_base_roll = g_angleDatas.roll;
    g_base_pitch = g_angleDatas.pitch;
    g_base_yaw = g_angleDatas.yaw;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();
    Servo_Reset();

    forward(40);
    delay_ms(50);

    // forward(40);
    // delay_ms(50);

    while (1)
    {
        u8 direction;
        direction = grayScale_detect();
        
        if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
            OLED_Refresh();

            turn_left();
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
            OLED_Refresh();

            turn_right();
        }
        else
        {
            OLED_ShowString(0, 20, (unsigned char *)"forward   ", 8, 1);
            OLED_Refresh();

            forward(40);
        }

        delay_ms(10);
    }

    return 0;
}

/**
 * @brief openmv色块识别
 *
 */
void UART5_IRQHandler(void)
{
    static u16 RxState = 0;
    static u16 pRxPacket = 0;
    if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
    {
        u8 RxData = USART_ReceiveData(UART5);
        if (RxState == 0)
        {
            if (RxData == '@')
            {
                RxState = 1;
                pRxPacket = 0;
            }
        }
        else if (RxState == 1)
        {
            if (RxData == '%')
            {
                RxState = 2;
            }
            else
            {
                g_Serial_RxPacket[pRxPacket] = RxData;
                pRxPacket++;
            }
        }
        else if (RxState == 2)
        {
            if (RxData == 'A')
            {
                RxState = 0;
                g_Serial_RxPacket[pRxPacket] = '\0';
                // Serial_RxFlag = 1;
            }
        }
        if (g_Serial_RxPacket[0] == '4')
        {
            stop();
            Servo_Action();
            delay_ms(1500);
            Servo_Reset();
            delay_ms(1500);
        }
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }   
}

/**
 * @brief TIM6中断函数
 *
 */
void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // 在这里执行20ms定时器触发后的操作
    {
        // 让编码器的值变成正整数
        ASR1.Fdb = (Num_Abs(Read_Speed(1)) - 48) / 6.3 + 10;
        ASR2.Fdb = (Num_Abs(Read_Speed(2)) - 48) / 6.3 + 10;
        ASR3.Fdb = (Num_Abs(Read_Speed(3)) - 48) / 6.3 + 10;
        ASR4.Fdb = (Num_Abs(Read_Speed(4)) - 48) / 6.3 + 10;

        ASR1.Err = ASR1.Ref - ASR1.Fdb;
        ASR2.Err = ASR2.Ref - ASR2.Fdb;
        ASR3.Err = ASR3.Ref - ASR3.Fdb;
        ASR4.Err = ASR4.Ref - ASR4.Fdb;

        pid_calc(&ASR1);
        pid_calc(&ASR2);
        pid_calc(&ASR3);
        pid_calc(&ASR4);

        output1 = ASR1.Out;
        output2 = ASR2.Out;
        output3 = ASR3.Out;
        output4 = ASR4.Out;
        PID_apply();

        jy901s_getData(&g_angleDatas);
        float pitch_max = 0;
        if (g_angleDatas.pitch >= pitch_max)
        {
            pitch_max = g_angleDatas.pitch;
        }

        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除中断标志位
    }
}
