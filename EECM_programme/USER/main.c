/*** 
 * @Author: Jonty ljt20030312@Outlook.com
 * @Date: 2023-08-28 18:47
 * @LastEditTime: 2023-09-14 17:57
 * @Description: 
 */

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"

int main(void)
{
    delay_init(168);
    OLED_Init();
}


