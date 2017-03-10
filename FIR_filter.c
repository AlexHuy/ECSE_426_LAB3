#include "FIR_filter.h"
 
//Performs the FIR filtering on pitch
float FIR_pitch(float input, struct FIR_coeff* coeff, int Order) {
	float moving_array[Order + 1];
	int filter_coeff_pointer;
	int counter = 0;
	int i;
	float sum = 0;
	float output;
	
	if(counter != Order) {
		moving_array[counter] = input;
		counter++;
		output = input;
	}
	else {
		for(i = 0; i < Order; i++) {
			if(i != Order - 1)
				moving_array[i] = moving_array[i+1];
			else
				moving_array[i] = input;
		}
		sum = 0;
		for(filter_coeff_pointer = 0; filter_coeff_pointer <= Order; filter_coeff_pointer++) {
			if(filter_coeff_pointer == 0)
				sum += coeff->b0 * moving_array[Order];
			else if(filter_coeff_pointer == 1) 
				sum += coeff->b1 * moving_array[Order - filter_coeff_pointer];
			else if(filter_coeff_pointer == 2)
				sum += coeff->b2 * moving_array[Order - filter_coeff_pointer];
			else if(filter_coeff_pointer == 3)
				sum += coeff->b3 * moving_array[Order - filter_coeff_pointer];
			else
				sum += coeff->b4 * moving_array[Order - filter_coeff_pointer];
		}
		output = sum;
	}
	
	return output;
}

//Performs the FIR filtering on roll
float FIR_roll(float input, struct FIR_coeff* coeff, int Order) {
	float moving_array[Order + 1];
	int filter_coeff_pointer;
	int counter = 0;
	int i;
	float sum = 0;
	float output;
	
	if(counter != Order) {
		moving_array[counter] = input;
		counter++;
		output = input;
	}
	else {
		for(i = 0; i < Order; i++) {
			if(i != Order - 1)
				moving_array[i] = moving_array[i+1];
			else
				moving_array[i] = input;
		}
		sum = 0;
		for(filter_coeff_pointer = 0; filter_coeff_pointer <= Order; filter_coeff_pointer++) {
			if(filter_coeff_pointer == 0)
				sum += coeff->b0 * moving_array[Order];
			else if(filter_coeff_pointer == 1) 
				sum += coeff->b1 * moving_array[Order - filter_coeff_pointer];
			else if(filter_coeff_pointer == 2)
				sum += coeff->b2 * moving_array[Order - filter_coeff_pointer];
			else if(filter_coeff_pointer == 3)
				sum += coeff->b3 * moving_array[Order - filter_coeff_pointer];
			else
				sum += coeff->b4 * moving_array[Order - filter_coeff_pointer];
		}
		output = sum;
	}
	
	return output;
}