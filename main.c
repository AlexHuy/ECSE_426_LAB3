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

/* Private variables ---------------------------------------------------------*/
float accelerometer_data[3];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	__HAL_RCC_TIM4_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();

	while (1){
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

void init_TIM4()
{
	TIM_Base_InitTypeDef tim_init;
	
	//Initialize the base init typedef of TIM
	//Desired Timer Frequency =  Timer Input Frequency / (Prescaler * Period) 
	//Desired = 2KHz
	//Timer Input Frequency = 84MHz
	tim_init.Prescaler = 1000;
	tim_init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init.Period = 42;
	tim_init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	TIM_OC_InitTypeDef tim_oc;
	
	//Initialize the oc init typedef of TIM
	tim_oc.OCMode = TIM_OCMODE_PWM1;
	
	
	tim4_handle.Instance = TIM4;
	tim4_handle.Init = tim_init;
	tim4_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	tim4_handle.Lock = HAL_UNLOCKED;
	tim4_handle.State = HAL_TIM_STATE_READY;
	
	HAL_TIM_PWM_MspInit(tim4_handle);
	HAL_TIM_PWM_Init(tim4_handle);
	HAL_TIM_PWM_ConfigChannel();
	HAL_TIM_PWM_Start_IT();
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

