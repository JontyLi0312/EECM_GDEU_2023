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

void turn_left(void);
void turn_right(void);
void forward(int16_t speed);
void backward(int16_t speed);
void stop(void);

u8 g_Serial_RxPacket[5] = {'0', '0', '0', '0', '0'};

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

    jy901s_angleData angleDatas;
    jy901s_getData(&angleDatas);
    float base_roll, base_pitch, base_yaw;
    base_roll = angleDatas.roll;
    base_pitch = angleDatas.pitch;
    base_yaw = angleDatas.yaw;

    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"Status: WORKING", 8, 1);
    OLED_Refresh();

    forward(40);
    delay_ms(50);

    while (1)
    {
        forward(40);
    }

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
 * @brief car turn left
 *
 */
void turn_left(void)
{
    motor1_control(1);
    PID_Move(18, 1);
    motor2_control(1);
    PID_Move(18, 2);
    motor3_control(2);
    PID_Move(18, 3);
    motor4_control(2);
    PID_Move(18, 4);
}

/**
 * @brief car turn right
 *
 */
void turn_right(void)
{
    motor1_control(2);
    PID_Move(18, 1);
    motor2_control(2);
    PID_Move(18, 2);
    motor3_control(1);
    PID_Move(18, 3);
    motor4_control(1);
    PID_Move(18, 4);
}

/**
 * @brief car forward
 *
 */
void forward(int16_t speed)
{
    motor1_control(1);
    PID_Move(speed, 1);
    motor2_control(1);
    PID_Move(speed, 2);
    motor3_control(1);
    PID_Move(speed, 3);
    motor4_control(1);
    PID_Move(speed, 4);
}

/**
 * @brief car backward
 *
 */
void backward(int16_t speed)
{
    motor1_control(2);
    PID_Move(speed, 1);
    motor2_control(2);
    PID_Move(speed, 2);
    motor3_control(2);
    PID_Move(speed, 3);
    motor4_control(2);
    PID_Move(speed, 4);
}

/**
 * @brief car stop
 *
 */
void stop(void)
{
    motor1_control(0);
    motor2_control(0);
    motor3_control(0);
    motor4_control(0);
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

        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除中断标志位
    }
}
