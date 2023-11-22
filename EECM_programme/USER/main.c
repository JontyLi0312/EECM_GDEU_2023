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
#include "servo_control.h"

u8 g_Serial_RxPacket[5] = { '0', '0', '0', '0', '0' };
u8 g_servo_start = 0;
int g_jy901s_stop = 0;
/**
 * @brief 小车绝对姿态数据
 *
 */
jy901s_angleData g_angleDatas;

int main(void)
{
    struct datas {
        /**
         * @brief 小车基准姿态数据
         *
         */
        float base_roll;
        float base_pitch;
        float base_yaw;

        /**
         * @brief 小车在(min, max)内则认为小车水平
         *
         */
        float horizontal_pitch_max;
        float horizontal_pitch_min;
        /**
         * @brief 小车角度>up_pitch则认为在爬升，小车角度<down_pitch则认为在向下
         *
         */
        float up_pitch;
        float down_pitch;

        /**
         * @brief 正常速度以及下阶梯速度
         *
         */
        int16_t normal_speed;
        int16_t low_speed;
    }baseDatas;
    baseDatas.normal_speed = 30;
    baseDatas.low_speed = 13;
    baseDatas.up_pitch = 10.00;
    baseDatas.down_pitch = -10.00;
    baseDatas.horizontal_pitch_max = 4.00;
    baseDatas.horizontal_pitch_min = -4.00;

    struct flag
    {
        /**
        * @brief 爬坡标志位,下坡标志位
        *
        */
        u8 upslope;
        u8 downhill;

        /**
        * @brief 暂停再启动标志位,低速行驶标志位
        *
        */
        u8 lowSpeed;
        u8 restart;

        /**
         * @brief 爬坡标志位
         *
         */
        u8 climb_stop;
    }flag;
    flag.downhill = 0;
    flag.upslope = 0;
    flag.lowSpeed = 0;
    flag.restart = 0;
    flag.climb_stop = 0;

    u8 direction;
    jy901s_angleData angle_correction;

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
    Servo_Reset();

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char*)"Status: WORKING", 8, 1);

    /**
     * @brief 获取小车基准姿态
     *
     */
    jy901s_getData(&g_angleDatas);
    baseDatas.base_pitch = g_angleDatas.pitch;
    baseDatas.base_roll = g_angleDatas.roll;
    baseDatas.base_yaw = g_angleDatas.yaw;

    while (1)
    {
        /**
         * @brief 得到小车相对值姿态数据
         *
         */
        angle_correction.pitch = g_angleDatas.pitch - baseDatas.base_pitch;

        if (angle_correction.pitch >= baseDatas.up_pitch)
        {
            flag.upslope++;
        }
        else if (angle_correction.pitch <= baseDatas.down_pitch)
        {
            flag.downhill++;
        }
        else if ((angle_correction.pitch < baseDatas.horizontal_pitch_max) &&
            (angle_correction.pitch > baseDatas.horizontal_pitch_min))
        {
            if ((flag.upslope != 0) || (flag.downhill != 0))
            {
                if (flag.upslope)
                {
                    flag.restart++;
                }
                else if (flag.downhill)
                {
                    flag.lowSpeed++;
                }
                flag.upslope = 0;
                flag.downhill = 0;
            }
        }

        if (flag.lowSpeed == 3)
        {
            flag.lowSpeed = 0;
            flag.restart = 0;
        }

        OLED_ShowNum(0, 40, flag.restart, 1, 8, 1);
        OLED_ShowNum(20, 40, flag.lowSpeed, 1, 8, 1);

        if (g_servo_start == 1)
        {
            g_jy901s_stop = 1;
            stop();
            Servo_Action();
            delay_ms(1500);
            Servo_Reset();
            delay_ms(1000);
            g_servo_start = 2;
            g_jy901s_stop = 0;

        }
        else if (g_servo_start == 2)
        {
            Servo_Init();
            delay_ms(3000);
        }

        direction = grayScale_detect();
        if (direction == 'l')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char*)" <- ", 8, 1);

            turn_left(30, 0);
        }
        else if (direction == 'r')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char*)" -> ", 8, 1);

            turn_right(30, 0);
        }
        else if (direction == 'L')
        {
            // turn left
            OLED_ShowString(0, 20, (unsigned char*)"<<--", 8, 1);

            turn_left(45, 10);
        }
        else if (direction == 'R')
        {
            // turn right
            OLED_ShowString(0, 20, (unsigned char*)"-->>", 8, 1);

            turn_right(45, 10);
        }
        else
        {
            OLED_ShowString(0, 20, (unsigned char*)" GO ", 8, 1);

            if ((flag.lowSpeed > 0) && (flag.lowSpeed < 3) && (flag.restart == 3))
            {
                if (flag.climb_stop > 1)
                {
                    stop();
                    delay_ms(2000);
                    flag.climb_stop = 0;
                }
                stop();
                delay_ms(60);
                forward(baseDatas.low_speed, baseDatas.low_speed);
                delay_ms(30);
            }
            else if (flag.restart == 2)
            {
                if (!flag.climb_stop)
                {
                    stop();
                    delay_ms(1000);
                    flag.climb_stop++;
                }

                forward(baseDatas.normal_speed, baseDatas.normal_speed + 8);
                flag.climb_stop++;
            }
            else if ((flag.lowSpeed == 1) && (flag.upslope > 0))
            {
                forward(baseDatas.normal_speed, baseDatas.normal_speed + 10);
            }
            else if ((flag.restart == 0) && (flag.lowSpeed == 0))
            {
                forward(baseDatas.normal_speed + 8, baseDatas.normal_speed + 8);
            }
            else
            {
                forward(baseDatas.normal_speed, baseDatas.normal_speed);
            }
        }
        OLED_Refresh();
    }

    return 0;
}

/**
 * @brief openmv color recognition
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
                
            }
        }
        if (g_Serial_RxPacket[0] == '4')
        {
            g_servo_start = 1;

            
        }
        
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
}

/**
 * @brief TIM6 interruption: motor PID & servo PID
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
        if (g_jy901s_stop == 0)
        {
            Servo_accomplish();
        }

        jy901s_getData(&g_angleDatas);

        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除中断标志位
    }
}
