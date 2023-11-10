# 2023 广东省大学生工科实验综合大赛\_全地形越障运送小车

MCU: STM32F407ZGT6

Keil project，base on AC6

## Category

1. Module
2. Pins
3. Code Description
4. Usage
5. Contributor
6. Contact
7. History

## Module

1. DC gear motor (JGB-520) with encoder
2. servo
3. jy901s
4. MP1584EN
5. OLED
6. 5 channel gray scale sensor

## Pins

### OpenMV

**UART5**

TX -> PC12

RX -> PD2

### DC gear motor 1

**TIM1**

encoder A -> PA8

encoder B -> PE11

**TIM3_CH1**

PWM -> PB4

**GPIO**

IN1 -> PC0

IN2 -> PC1

### DC gerat motor 2

**TIM8**

encoder A -> PC6

encoder B -> PC7

**TIM3_CH2**

PWM -> PA7

**GPIO**

IN1 -> PC2

IN2 -> PC3

### DC gerat motor 3

**TIM2**

encoder A -> PA15

encoder B -> PB3

**TIM3_CH3**

PWM -> PC8

**GPIO**

IN1 -> PC4

IN2 -> PC5

### DC gerat motor 4

**TIM4**

encoder A -> PB6

encoder B -> PB7

**TIM3_CH4**

PWM -> PC9

**GPIO**

IN1 -> PC10

IN2 -> PC11

### Servo

**TIM5**

top servo PWM -> PA2

buttom servo PWM -> PA1

### OLED

**IIC**

SCL -> PG12

SDA -> PG11

### jy901s

**USART2**

RX -> PA3

TX -> PD5

### 5 channel gray scale sensor

**GPIO**

left -> PB11

mid_left -> PB12

mid -> PB13

mid_right -> PE13

right -> PB15

### stop button

**GPIO**

stop -> PG14

## Code Description

### Hardware

-   OLED

    -   OLED 显示

-   5ChannelGrayScaleSensor

    -   五路灰度扫描循迹并返回方向

-   jy901s

    -   获取小车姿态
    -   私有通信协议

-   motor

    -   编码器初始化
    -   电机控制
    -   PID 算法

-   OPENMV

    -   颜色识别

-   servo

    -   舵机 PWM
    -   舵机控制

### System

-   Delay

    -   延时

-   motor_PWM

    -   电机 PWM

-   sys

    -   头文件

-   Timer

    -   定时中断初始化

-   uart5

    -   串口 5 初始化

-   usart

    -   串口 1 初始化
    -   printf 函数重定义

-   usart2

    -   串口 2 初始化

### User

-   main
    -   主程序

## Usage

1. 通电
2. 按下 reset 按钮
3. 越过障碍
4. OPENMV 识别色卡
5. 匹配色卡
6. 倾倒弹珠

## Contributor

Jonty Li, Tender Zephyr, coolersf

## History

version 0.1.0: initialize

version 0.2.0：增加开始按钮与颜色选择按钮

version 0.3.0：增加 LED 显示颜色选择结果

version 0.4.0：移除 LED 显示颜色选择结果

version 0.5.0：增加对直流减速编码电机的支持

version 0.6.0：增加舵机控制

version 0.7.0：增加 jy901s 获取角度信息

version 0.8.0：增加五路灰度传感器获取方向信息

version 0.9.0：电机 PID

version 0.10.0：OPENMV 颜色识别
