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
2. 2D Motorized Head (with servo - )
3. jy901s
4. MP1584EN
5. OLED
6. key
7. 5 channel gray scale sensor

## Pins

### OpenMV

TX -> PC12

RX -> PD2

### DC gear motor 1

encoder A -> PA8

encoder B -> PA9

PWM -> PA6

IN1 -> PC0

IN2 -> PC1

### DC gerat motor 2

encoder A -> PC6

encoder B -> PC7

PWM -> PA7

IN1 -> PC2

IN2 -> PC3

### DC gerat motor 3

encoder A -> PA15

encoder B -> PB3

PWM -> PC8

IN1 -> PC4

IN2 -> PC5

### DC gerat motor 4

encoder A -> PA0

encoder B -> PA1

PWM -> PC9

IN1 -> PC10

IN2 -> PC11

### 2D Motorized Head Servo

top servo PWM -> PB7

buttom servo PWM -> PB6

### OLED

SCL -> PG12

SDA -> PD5

### key

start key -> PB4

### jy901s

TX -> PA2

RX -> PA3

### 5 channel gray scale sensor

left -> PB11

mid_left -> PB12

mid -> PB13

mid_right -> PE13

right -> PB15

## Code Description

### Hardware

-   OLED

### System

-   Delay

### User

-   main

## Usage

1. press start buttom
2.

## Contributor

Jonty Li, Tender Zephyr, coolersf

## History

version 0.1.0: initialize
