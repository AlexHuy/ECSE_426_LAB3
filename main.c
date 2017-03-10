/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "accelerometer.h"
#include "keypad.h"
#include "timer.h"
#include "math.h"

/* Private variables ---------------------------------------------------------*/
float accelerometer_data[3];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

int main(void)
{	
	float pitch, roll;
	float desired_pitch, desired_roll;
	float pitch_diff, roll_diff;
	int output[] = {0, 0, 0};
	int i;
  /* MCU Configuration----------------------------------------------------------*/
	int target;
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	__HAL_RCC_TIM4_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	
	init_accelerometer();
	while (1)
	{
		if(accel_rdy_flag == 1)
		{
			/*order_key(output);
			if (key_stage == 4){
				key_stage = 0;
				printf("Angle entered is");
				for( i = 0; i<3; i++){
					printf("%d", output[i]);
			}
			target = output[0]*100 + output[1]*10 +output[2];
		}*/
			/*printf("Data on x: %f\n", accelerometer_data[0]);
			printf("Data on y: %f\n", accelerometer_data[1]);
			printf("Data on z: %f\n", accelerometer_data[2]);*/
			
			calibrate_accelerometer_data(accelerometer_data);
			
			pitch = calc_pitch(accelerometer_data);
			printf("Pitch is: %f\n", pitch);
			pitch_diff = desired_pitch - pitch;
			if(pitch_diff == 0)
			{
				set_DC(0);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(pitch_diff) < 45 && fabs(pitch_diff) > 0 )
			{
				set_DC(1);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(pitch_diff) < 90 && fabs(pitch_diff) > 45)
			{
				set_DC(2);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(pitch_diff) < 180 && fabs(pitch_diff) > 90)
			{
				set_DC(3);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			
			roll = calc_roll(accelerometer_data);
			printf("Roll is: %f\n", roll);
			roll_diff = desired_roll - roll;
			if(roll_diff == 0)
			{
				set_DC(0);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(roll_diff) < 45 && fabs(roll_diff) > 0 )
			{
				set_DC(1);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(roll_diff) < 90 && fabs(roll_diff) > 45)
			{
				set_DC(2);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			else if(fabs(roll_diff) < 180 && fabs(roll_diff) > 90)
			{
				set_DC(3);
				//HAL_TIM_PWM_ConfigChannel(&tim4_handle, &tim_oc, TIM_CHANNEL_1);
			}
			
			accel_rdy_flag = 0;
		}
	}
}

/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	LIS3DSH_ReadACC(accelerometer_data);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif

