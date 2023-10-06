/***
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-10-06 16:40
 * @LastEditTime: 2023-10-06 16:41
 * @Description: led显示颜色选择结果
 */
#ifndef __LED_H
#define __LED_H
#include "sys.h"

void led_init(void);
void set_red_led(uint8_t status);
void set_blue_led(uint8_t status);
void set_green_led(uint8_t status);

#endif
