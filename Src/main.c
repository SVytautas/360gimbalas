

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart2_dbg.h"
#include "btns_control.h"
#include "stepper_controls.h"



void SystemClock_Config(void);
static void MX_GPIO_Init(void);


TIM_HandleTypeDef htim1;
static void MX_TIM1_Init(void);

int main(void)
{
	
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
	
	MX_TIM1_Init();
	
  BTNS_CONTROL_init();
	
	
	uart2_dbg_init();
	
  HAL_Delay(500);
  
	STEPPER_CONTROLS_ST2_DISABLE();
	STEPPER_CONTROLS_ST1_DISABLE();
	/* Infinite loop */
  while (1)
  {
		uart2_dbg_handler();
	  uart2_dbg_cmd_handler();
		
		STEPPER_CONTROLS_handler();
  }
	
}


uint32_t cnt_event;
uint32_t cnt_100hz;

void system_tick_1khz(void)
{
	//sys tick 1khz
	//STEPPER_CONTROLS_handler_1kHz();
	
	cnt_event++;
	if (cnt_event>=500)
	{
	   cnt_event=0;
		//*
		//HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_RESET);
		//if (HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)==0)
		//{
		//	PRINTF("set");
		//	HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_SET);
		//  HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_SET);
		//}
	}
	
	//100Hz eventas
	cnt_100hz++;
	if (cnt_100hz>=10)
	{
		cnt_100hz=0;
		
		BTNS_CONTROL_handler_100Hz();
		
	}
	

}


/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 639; //100khz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 19; //5khz
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
	if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */

uint32_t counter_a = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	STEPPER_CONTROLS_handler_5kHz();
  counter_a++;
	if (counter_a>5000)
	{
		counter_a=0;
		//PRINTF("!");
		
		
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED3_Pin|LED2_Pin|LED5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Cam_Focus_Pin|Cam_Realise_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_Maple_Pin|ST1_DIR_Pin|ST1_ENA_Pin|ST1_STEP_Pin
                          |LED1_Pin|LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ST2_DIR_Pin|ST2_ENA_Pin|ST2_STEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED3_Pin LED2_Pin LED5_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin|LED5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : END_ST2_Pin BTN2_Pin BTN1_Pin */
  GPIO_InitStruct.Pin = END_ST2_Pin|BTN2_Pin|BTN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Cam_Focus_Pin Cam_Realise_Pin */
  GPIO_InitStruct.Pin = Cam_Focus_Pin|Cam_Realise_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : END_ST1_Pin */
  GPIO_InitStruct.Pin = END_ST1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(END_ST1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Maple_Pin LED1_Pin LED4_Pin */
  GPIO_InitStruct.Pin = LED_Maple_Pin|LED1_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ST2_DIR_Pin ST2_ENA_Pin ST2_STEP_Pin */
  GPIO_InitStruct.Pin = ST2_DIR_Pin|ST2_ENA_Pin|ST2_STEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ST1_DIR_Pin ST1_ENA_Pin ST1_STEP_Pin */
  GPIO_InitStruct.Pin = ST1_DIR_Pin|ST1_ENA_Pin|ST1_STEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_maple_Pin */
  GPIO_InitStruct.Pin = Button_maple_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_maple_GPIO_Port, &GPIO_InitStruct);

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
