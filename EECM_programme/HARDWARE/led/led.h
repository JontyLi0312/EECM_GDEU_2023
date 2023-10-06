#ifndef __LED_H
#define __LED_H
#include "sys.h"

void led_init(void);
void set_red_led(uint8_t status);
void set_blue_led(uint8_t status);
void set_green_led(uint8_t status);

#endif
