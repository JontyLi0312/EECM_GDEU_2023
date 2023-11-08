#ifndef __DRIVE_H
#define __DRIVE_H
#include "sys.h"

void turn_left(int16_t speed);
void turn_right(int16_t speed);
void forward(int16_t speed);
void backward(int16_t speed);
void stop(void);

#endif