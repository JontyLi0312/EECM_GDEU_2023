#include "drive.h"
#include "encoder_PID.h"
#include "motor.h"

void turn_left(int16_t f_speed, int16_t b_speed);
void turn_right(int16_t f_speed, int16_t b_speed);
void forward(int16_t speed);
void backward(int16_t speed);
void stop(void);

/**
 * @brief 左转程序
 *
 * @param f_speed 前进轮速度
 * @param b_speed 后退轮速度
 */
void turn_left(int16_t f_speed, int16_t b_speed)
{
    motor1_control(1);
    PID_Move(f_speed, 1);
    motor2_control(1);
    PID_Move(f_speed, 2);
    motor3_control(2);
    PID_Move(b_speed, 3);
    motor4_control(2);
    PID_Move(b_speed, 4);
}

/**
 * @brief 右转程序
 *
 * @param f_speed 前进轮速度
 * @param b_speed 后退轮速度
 */
void turn_right(int16_t f_speed, int16_t b_speed)
{
    motor1_control(2);
    PID_Move(b_speed, 1);
    motor2_control(2);
    PID_Move(b_speed, 2);
    motor3_control(1);
    PID_Move(f_speed, 3);
    motor4_control(1);
    PID_Move(f_speed, 4);
}

/**
 * @brief 前进
 *
 * @param speed 前进速度
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
 * @brief 后退速度
 *
 * @param speed
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
 * @brief car short brake
 *
 */
void stop(void)
{
    motor1_control(0);
    motor2_control(0);
    motor3_control(0);
    motor4_control(0);
}
