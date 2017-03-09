#include "accelerometer.h"
#include "lis3dsh.h"

void init_accelerometer() 
{
	LIS3DSH_InitTypeDef accel_init;
	
	accel_init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
	accel_init.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	accel_init.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;
	accel_init.AA_Filter_BW = LIS3DSH_AA_BW_50;
	accel_init.Full_Scale = LIS3DSH_FULLSCALE_2;
	accel_init.Self_Test = LIS3DSH_SELFTEST_NORMAL ;
}