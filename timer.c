#include "timer.h"
#include "main.h"

TIM_Base_InitTypeDef tim_init;
TIM_OC_InitTypeDef tim_oc;

void init_TIM4()
{
	//Initialize the base init typedef of TIM
	//Desired Timer Frequency =  Timer Input Frequency / (Prescaler * Period) 
	//Desired = 2KHz
	//Timer Input Frequency = 84MHz
	tim_init.Prescaler = 21;
	tim_init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init.Period = 2000;
	tim_init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	//Initialize the oc init typedef of TIM
	tim_oc.OCMode = TIM_OCMODE_PWM1;
	tim_oc.Pulse = 1000; //Initialize duty cycle at 50%
	
	tim4_handle.Instance = TIM4;
	tim4_handle.Init = tim_init;
	tim4_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1 | HAL_TIM_ACTIVE_CHANNEL_2 | HAL_TIM_ACTIVE_CHANNEL_3 | HAL_TIM_ACTIVE_CHANNEL_4;
	tim4_handle.Lock = HAL_UNLOCKED;
	tim4_handle.State = HAL_TIM_STATE_READY;
	
	//Init PWM functions
	HAL_TIM_PWM_MspInit(&tim4_handle);
	HAL_TIM_PWM_Init(&tim4_handle);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim4_handle, TIM_CHANNEL_4);
	
	//Enable IRQ for TIM4 and set its priority
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
}

//Initialize GPIO pin for built in LED
void init_LED()
{
	GPIO_InitTypeDef GPIOD_init;
	
	GPIOD_init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIOD_init.Mode = GPIO_MODE_AF_PP;
	GPIOD_init.Pull = GPIO_NOPULL;
	GPIOD_init.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIOD_init.Alternate = GPIO_AF2_TIM4;
	
	HAL_GPIO_Init(GPIOD, &GPIOD_init);
}

//Change the Duty Cycle depending on the stages
void set_DC(int stage)
{
	if(stage == 0) //Stage 0 corresponds to 0%, LED off
		tim_oc.Pulse = 0;
	else if(stage == 1) //Stage 1 corresponds to 25%
		tim_oc.Pulse = 500;
	else if(stage == 2) //Stage 2 corresponds to 50%
		tim_oc.Pulse = 1000;
	else if(stage == 3) //Stage 3 corresponds to 75%
		tim_oc.Pulse = 1500;
	else
		tim_oc.Pulse = 2000; //Others corresponds to 100%, LED fully on
}

