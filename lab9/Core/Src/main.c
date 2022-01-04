/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rxBuffer[20];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		static unsigned char uRx_Data[1024] = { 0 };
		static unsigned char uLength = 0;
		static unsigned char a[25] = { 0 };
		static unsigned char b[25] = { 0 };
		static unsigned char c[25] = { 0 };
		static unsigned char d[25] = { 0 };
		static unsigned char e[25] = { 0 };
		static unsigned char f[25] = { 0 };
		static unsigned char g[25] = { 0 };
		static unsigned char h[25] = { 0 };
		static unsigned char i[25] = { 0 };
		static unsigned char color[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		if (rxBuffer[0] == '\n') {
			HAL_UART_Transmit(&huart1, uRx_Data, uLength, 0xffff);

			int p = uLength / 24+1;
			int l = uLength % 24;
			for (int m = 0; m < p; m++) {
				for (int k = 0; k < 25; k++) {
					a[k] = b[k];
					b[k] = c[k];
					c[k] = d[k];
					d[k] = e[k];
					e[k] = f[k];
					f[k] = g[k];
					g[k] = h[k];
					h[k] = i[k];
				}
				if (m==p-1){
					for (int t=0;t<24-l;t++){
						i[t]='\40';
					}
					for (int t= 0;t<l;t++){
						i[24-l+t]=uRx_Data[t+m*24];
					}
					i[24] = 0;
				}else{
					for (int k = 0;k<25;k++){
						i[k] = uRx_Data[k+m*24];
					}
				}
				for (int o = 0; o < 8; o++) {
					color[o] = color[o + 1];
				}
				color[8] = 0;
			}

//			for (int k = 0; k < 25; k++) {
//				a[k] = b[k];
//				b[k] = c[k];
//				c[k] = d[k];
//				d[k] = e[k];
//				e[k] = f[k];
//				f[k] = g[k];
//				g[k] = h[k];
//				h[k] = i[k];
//			}

//			int l = uLength % 24;
//			for (int k = 0; k < 24 - l; k++) {
//				i[k] = '\40';
//			}
//			for (int k = 0; k < l; k++) {
//				i[24 - l + k] = uRx_Data[k];
//			}
//
//			i[24] = 0;
//			for (int o = 0; o < 8; o++) {
//				color[o] = color[o + 1];
//			}
//			color[8] = 0;

			if (uLength == 12 && uRx_Data[0] == 't' && uRx_Data[1] == 'e'
					&& uRx_Data[2] == 'm' && uRx_Data[3] == 'p'
					&& uRx_Data[4] == 'e' && uRx_Data[5] == 'r'
					&& uRx_Data[6] == 'a' && uRx_Data[7] == 't'
					&& uRx_Data[8] == 'u' && uRx_Data[9] == 'r'
					&& uRx_Data[10] == 'e' && uRx_Data[11] == '\r') {

				HAL_UART_Transmit(&huart1, uRx_Data, uLength, 0xffff);

				HAL_ADC_Start(&hadc1);

				HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

				uint16_t raw = HAL_ADC_GetValue(&hadc1);
				float voltage = (1.43 - (raw * (3.3 / 4096))) / 4.3 + 25;

				char msg[20];
				sprintf(msg, "%.6f\r\n", voltage);
				HAL_UART_Transmit(&huart1, msg, 9, 0xffff);

				for (int k = 0; k < 25; k++) {
					a[k] = b[k];
					b[k] = c[k];
					c[k] = d[k];
					d[k] = e[k];
					e[k] = f[k];
					f[k] = g[k];
					g[k] = h[k];
					h[k] = i[k];
				}

				for (int k = 0; k < 9; k++) {
					i[k] = msg[k];
				}
				i[9] = 0;
				for (int o = 0; o < 8; o++) {
					color[o] = color[o + 1];
				}
				color[8] = 1;
			}

			LCD_Clear(WHITE);
			POINT_COLOR = BLACK;
			LCD_DrawRectangle(20, 30, 220, 300);
			POINT_COLOR = (color[0] == 0) ? RED : BLUE;
			LCD_ShowString(23, 35, 185, 10, 16, (uint8_t*) a);
			POINT_COLOR = (color[1] == 0) ? RED : BLUE;
			LCD_ShowString(23, 65, 185, 10, 16, (uint8_t*) b);
			POINT_COLOR = (color[2] == 0) ? RED : BLUE;
			LCD_ShowString(23, 95, 185, 10, 16, (uint8_t*) c);
			POINT_COLOR = (color[3] == 0) ? RED : BLUE;
			LCD_ShowString(23, 125, 185, 10, 16, (uint8_t*) d);
			POINT_COLOR = (color[4] == 0) ? RED : BLUE;
			LCD_ShowString(23, 155, 185, 10, 16, (uint8_t*) e);
			POINT_COLOR = (color[5] == 0) ? RED : BLUE;
			LCD_ShowString(23, 185, 185, 10, 16, (uint8_t*) f);
			POINT_COLOR = (color[6] == 0) ? RED : BLUE;
			LCD_ShowString(23, 215, 185, 10, 16, (uint8_t*) g);
			POINT_COLOR = (color[7] == 0) ? RED : BLUE;
			LCD_ShowString(23, 245, 185, 10, 16, (uint8_t*) h);
			POINT_COLOR = (color[8] == 0) ? RED : BLUE;
			LCD_ShowString(23, 275, 185, 10, 16, (uint8_t*) i);

			uLength = 0;
		} else {
			uRx_Data[uLength] = rxBuffer[0];
//			HAL_UART_Transmit(&huart1, rxBuffer[0], 1, 0xffff);
			uLength++;
		}
	}
}

void USART1_IRQHandler(void) {
	/* USER CODE BEGIN USART1_IRQn 0 */
	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */
	HAL_UART_Receive_IT(&huart1, (uint8_t*) rxBuffer, 1);
	/* USER CODE END USART1_IRQn 1 */
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	LCD_Init();
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, (uint8_t*) rxBuffer, 1);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1) {
	}
//  uint8_t x = 0;
//  while (1){
//    /* USER CODE END WHILE */
//
//    /* USER CODE BEGIN 3 */
//	  switch (x) {
//		  case 0:
//			  LCD_Clear(WHITE);
//			  BACK_COLOR = WHITE;
//			  break;
//		  case 1:
//			  LCD_Clear(BLACK);
//			  BACK_COLOR = BLACK;
//			  break;
//		  case 2:
//			  LCD_Clear(BLUE);
//			  BACK_COLOR = BLUE;
//			  break;
//		  case 3:
//			  LCD_Clear(RED);
//			  BACK_COLOR = RED;
//			  break;
//		  case 4:
//			  LCD_Clear(MAGENTA);
//			  BACK_COLOR = MAGENTA;
//			  break;
//		  case 5:
//			  LCD_Clear(GREEN);
//			  BACK_COLOR = GREEN;
//			  break;
//		  case 6:
//			  LCD_Clear(CYAN);
//			  BACK_COLOR = CYAN;
//			  break;
//		  case 7:
//			  LCD_Clear(YELLOW);
//			  BACK_COLOR = YELLOW;
//			  break;
//		  case 8:
//			  LCD_Clear(BRRED);
//			  BACK_COLOR = BRRED;
//			  break;
//		  case 9:
//			  LCD_Clear(GRAY);
//			  BACK_COLOR = GRAY;
//			  break;
//		  case 10:
//			  LCD_Clear(LGRAY);
//			  BACK_COLOR = LGRAY;
//			  break;
//		  case 11:
//			  LCD_Clear(BROWN);
//			  BACK_COLOR = BROWN;
//			  break;
//	  }
//	  POINT_COLOR = RED;
//	  LCD_ShowString(30, 40, 200, 24, 24, (uint8_t*) "Mini STM32 ^_^");
//	  LCD_ShowString(30, 70, 200, 16, 16, (uint8_t*) "TFTLCD TEST");
//	  POINT_COLOR = BLACK;
//	  LCD_DrawRectangle(30, 150, 210, 190);
//	  LCD_Fill(31, 151, 209, 189, YELLOW);
//
//	  HAL_ADC_Start(&hadc1);
//	  // Wait for regular group conversion to be completed
//	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//	  // Get ADC value
//	  raw = HAL_ADC_GetValue(&hadc1); // the voltage should be raw * (3.3/4096)(12bits)
//	  float voltage = (1.43 - (raw * (3.3/4096))) / 4.3 + 25;
//	  // Convert to string and print
//	  sprintf(msg, "%.6f\r\n", voltage);
//
//	  LCD_ShowString(30, 105, 200, 16, 16, (uint8_t*) msg);
//	  x++;
//	  if (x == 12)
//		  x = 0;
//
//	  HAL_Delay(2000);
//  }
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
