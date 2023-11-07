#ifndef __DRIVE_H
#define __DRIVE_H
#include "sys.h"

void turn_left(void);
void turn_right(void);
void forward(int16_t speed);
void backward(int16_t speed);
void stop(void);
// test
#endif