#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H

#include "lis3dsh.h"
#include "stm32f4xx_it.h"
#include <math.h>

void init_accelerometer();
void calibrate_accelerometer_data(float accelerometer_d[3]);
float calc_pitch(float accelerometer_data[3]);
float calc_roll(float accelerometer_data[3]);
float radToDegree(float rad);

#endif