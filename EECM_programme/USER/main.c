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
#include "motor_encoder.h"
#include "jy901s.h"
#include "5graysacle.h"
#include "Timer.h"
#include "encoder_PID.h"
#include "UART5.h"
#include "servo_apply.h"
#include "drive.h"

u8 g_Serial_RxPacket[5] = {'0', '0', '0', '0', '0'};
u16 g_flag = 0;

/**
 * @brief 小车姿态数据
 *
 */
jy901s_angleData g_angleDatas;
/**
 * @brief 小车姿态基准值
 *
 */

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

    jy901s_getData(&g_angleDatas);
    float base_roll;
    float base_pitch;
    float base_yaw;
    base_roll = g_angleDatas.roll;
    base_pitch = g_angleDatas.pitch;
    base_yaw = g_angleDatas.yaw;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();
    Servo_Reset();

    // forward(40);
    // delay_ms(50);

    // while (1)
    // {
    //     forward(30);
    // }

    while (1)
    {
        // jy901s_angleData angle_correction;
        // angle_correction.pitch = g_angleDatas.pitch - base_pitch;
        // angle_correction.roll = g_angleDatas.roll - base_roll;
        // angle_correction.yaw = g_angleDatas.yaw - base_yaw;

        // float pitch_max = 0;
        // u8 climb_flag = 0;
        // if (angle_correction.pitch >= pitch_max)
        // {
        //     pitch_max = angle_correction.pitch;
        // }
        // OLED_ShowNum(0, 30, (int32_t)pitch_max, 4, 8, 1);
        // OLED_Refresh();

        // int angle_flag;
        // angle_flag = 0;
        // if (angle_correction.pitch >= 10)
        // {
        //     if (angle_flag < 1)
        //     {
        //         climb_flag++;
        //     }
        //     angle_flag++;
        // }
        // else
        // {
        //     angle_flag = 0;
        // }
        // OLED_ShowChar(0, 40, climb_flag, 8, 1);
        // OLED_Refresh();

        u8 direction;
        direction = grayScale_detect();
        if (g_flag == 1)
        {
            stop();
            Servo_Action();
            delay_ms(5000);
            g_flag = 2;
        }
        else if (g_flag == 2)
        {
            Servo_Init();
            delay_ms(3000);
        }
        if (direction == 'l')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
            OLED_Refresh();

            turn_left(40);
        }
        else if (direction == 'r')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
            OLED_Refresh();

            turn_right(40);
        }
        if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char *)"turn left ", 8, 1);
            OLED_Refresh();

            turn_left(80);
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char *)"turn right", 8, 1);
            OLED_Refresh();

            turn_right(80);
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
            g_flag = 1;
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

        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除中断标志位
    }
}
