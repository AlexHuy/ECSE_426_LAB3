#include "timer.h"
#inlcude "main.h"

void init_TIM4()
{
	TIM_Base_InitTypeDef tim_init;
	
	//Initialize the base init typedef of TIM
	//Desired Timer Frequency =  Timer Input Frequency / (Prescaler * Period) 
	//Desired = 2KHz
	//Timer Input Frequency = 84MHz
	tim_init.Prescaler = 21;
	tim_init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init.Period = 2000;
	tim_init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	TIM_OC_InitTypeDef tim_oc;
	
	//Initialize the oc init typedef of TIM
	tim_oc.OCMode = TIM_OCMODE_PWM1;
	tim_oc.Pulse = 1000; //Initialize duty cycle at 50%
	
	tim4_handle.Instance = TIM4;
	tim4_handle.Init = tim_init;
	tim4_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	tim4_handle.Lock = HAL_UNLOCKED;
	tim4_handle.State = HAL_TIM_STATE_READY;
	
	HAL_TIM_PWM_MspInit(tim4_handle);
	HAL_TIM_PWM_Init(tim4_handle);
	HAL_TIM_PWM_ConfigChannel(tim4_handle, tim_oc, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(tim4_handle, TIM_CHANNEL_1);
}