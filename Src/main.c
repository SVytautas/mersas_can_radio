/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart2_dbg.h"
#include "CAN_MERSEDES_83kbps.h"
#include "BT_control_gpio.h"
#include "CD4052.h"
#include "Mercedes_buttons_control.h"

void SystemClock_Config(void); //clock configas
static void MX_GPIO_Init(void); //kolkas paliekamas

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	//HAL ir sysClock configai butini
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
	
	
	uart2_dbg_init(); //Debug serial initas

	BT_control_gpio_init();
	CD4052_init();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	HAL_Delay(10); //2s delay kol BT isijungs
	//HAL_Delay(8000); //8s Becker 6021 delay kol BT isijungs
  
	
	CAN_mersas_init();
  Mercedes_buttons_control_init();
	
	PRINTF("START CPU: %lu\r\n",HAL_RCC_GetSysClockFreq());
	
	/* Infinite loop */
	//CD4052_SET_RADIO();
	//CD4052_SET_BT_OUT();
	
	
	
  while (1)
  {
		//debug UArt handleris
		uart2_dbg_handler();
	  uart2_dbg_cmd_handler();
		
		CAN_mersedes_handler();
		
		BT_control_gpio_handler(); //gali buti ir tucias 
		Mercedes_buttons_control_handler();
  }
	
}

uint32_t cnt_event;
extern volatile bool get_radio_code;
void system_tick_1khz(void)
{
	//sys tick 1khz
	BT_control_gpio_systick();
	CD4052_tick_1khz();
	Mercedes_buttons_control_systick();
	
	systick1s();
	//2s eventas
	cnt_event++;
	if (cnt_event>=2000)
	{
	   cnt_event=0;
		if (get_radio_code)
		{
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
		}
		 
	}
	
	
	

}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}





/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);

  /*Configure GPIO pin : PB11 */ //POWER BT ON SET 
  GPIO_InitStruct.Pin = GPIO_PIN_11; //power 0, audio Radio/BT PB10
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
