#ifndef __FIR_FILTER_H
#define __FIR_FILTER_H

#include <stdint.h>
#include <math.h>
#include <stdint.h>

struct FIR_coeff {
	float b0;
	float b1;
	float b2;
	float b3;
	float b4;   
};
//Defines the structure of the coefficients for the FIR filter.
float FIR_C(float input, struct FIR_coeff* coeff, int Order);

#endif