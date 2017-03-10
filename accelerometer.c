#include "accelerometer.h"
#include "math.h"
void init_accelerometer() 
{
	LIS3DSH_InitTypeDef accel_init;
	LIS3DSH_DRYInterruptConfigTypeDef accel_it_config;
	
	//Initialize accelerometer parameters
	accel_init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
	accel_init.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	accel_init.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;
	accel_init.AA_Filter_BW = LIS3DSH_AA_BW_50;
	accel_init.Full_Scale = LIS3DSH_FULLSCALE_2;
	accel_init.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	
	//Set accelerometer interrupt config
	accel_it_config.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	accel_it_config.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	accel_it_config.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	//Initialize function from drivers
	LIS3DSH_Init(&accel_init);
	LIS3DSH_DataReadyInterruptConfig(&accel_it_config);
	
	//Enable IRQ for accelerometer and set its priority
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
}
//Calculates the pitch of the board using the 3 axis.
float calc_pitch(float accelerometer_data[3]){
	float pitch = atanf(accelerometer_data[0]/(sqrtf(accelerometer_data[1]*accelerometer_data[1] + accelerometer_data[2]*accelerometer_data[2])));
	return pitch;
}

float calc_roll(float accelerometer_data[3]){
	float roll = atanf(accelerometer_data[1]/(sqrtf(accelerometer_data[0]*accelerometer_data[0] + accelerometer_data[2]*accelerometer_data[2])));
	return roll;
}
