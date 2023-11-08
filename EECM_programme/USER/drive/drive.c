#include "drive.h"
#include "encoder_PID.h"
#include "motor.h"

void turn_left(int16_t speed);
void turn_right(int16_t speed);
void forward(int16_t speed);
void backward(int16_t speed);
void stop(void);

/**
 * @brief car turn left
 *
 */
void turn_left(int16_t speed)
{
    motor1_control(1);
    PID_Move(speed, 1);
    motor2_control(1);
    PID_Move(speed, 2);
    motor3_control(2);
    PID_Move(10, 3);
    motor4_control(2);
    PID_Move(10, 4);
}

/**
 * @brief car turn right
 *
 */
void turn_right(int16_t speed)
{
    motor1_control(2);
    PID_Move(10, 1);
    motor2_control(2);
    PID_Move(10, 2);
    motor3_control(1);
    PID_Move(speed, 3);
    motor4_control(1);
    PID_Move(speed, 4);
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
