/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "font.h"
#include "bitmap.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */









  void LCD_DATA (uint8_t data)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET);
	  HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin, GPIO_PIN_SET);

  }

  void LCD_CMD (uint8_t cmd)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin | LCD_DC_Pin, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin | LCD_DC_Pin, GPIO_PIN_SET);

  }

  void LCD_ON (void)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET);  // LCD RST ACTIVE
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET);    // LCD ACTIVE
	  HAL_GPIO_WritePin(GPIOA, LCD_LIGHT_Pin, GPIO_PIN_SET);   // Light active

	  LCD_CMD(0x21);
	  LCD_CMD(0xc8);
	  LCD_CMD(0x06);
	  LCD_CMD(0x11);
	  LCD_CMD(0x20);
	  LCD_CMD(0x08);
	  LCD_CMD(0x09);
	  LCD_CMD(0x0d);
	  LCD_CMD(0xc8);

	  HAL_Delay(1000);
	  LCD_CMD(0x0c);
	  HAL_Delay(1000);

  }

  void LCD_CLEAR (void)
  {

	  uint8_t position_y = 0x40;
	  uint8_t y = 0;



	  LCD_CMD(0x80);
	  LCD_CMD(position_y);

	  while(y < 8)
	  {
		  for (uint8_t a = 0; a < 84; a++ )
		  	  {
		  		  LCD_DATA(0x00);
		  	  }
		  LCD_CMD(0x80);
		  y ++;
		  position_y ++;
		  LCD_CMD(position_y);

	  }
  }

  void LCD_SEND_PICTURE (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t picture[])        // location x, location y, width pixels, height pixels, picture
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  uint8_t width_1;
	  uint8_t end_width;
	  uint16_t element;
	  uint16_t koniec;      //ostatni bajt obrazka

	  element=0;

	  end_width=x+width;
	  height=height/8;

	  	width_1=x;


	  	koniec=height*width;


	  	address_x=0x80;             // stała wartoć którš LCD interpretuje jako adres
	  	address_y=0x40;             // jak wyżej
	  	address_x=address_x+x;
	  	address_y=address_y+y;

	  	LCD_CMD(address_x);
	  	LCD_CMD(address_y);


	  while(1)
	  {
		  LCD_DATA(picture[element]);
		  element++;
		  width_1++;
		  if(element==koniec) break;
		  if(width_1==end_width) {width_1=x; address_y++; LCD_CMD(address_x); LCD_CMD(address_y); };
	  }
  }

  void LCD_SEND_DIGIT (uint8_t x, uint8_t y, int digit) // location x, location y, data
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  unsigned char argument_number;
	     // rest of the division
	  uint16_t rest;
	  uint8_t a;
	  a = 0;




	  address_x=0x80;             // stała wartość którš LCD interpretuje jako adres
	  address_y=0x40;             // jak wyżej
	  address_x=address_x+x;
	  address_y=address_y+y;

	  LCD_CMD(address_x);   // location of cursor
	  LCD_CMD(address_y);   //

	  rest = digit % 10;
	  digit = digit - rest;
	  while(digit > 9) {digit = digit / 10;}
	  argument_number = 0;
	  for(a = 6; a>0; a--){LCD_DATA(digits[digit][argument_number]); argument_number ++;}
	  argument_number = 0;
	  for(a = 6; a>0; a--){LCD_DATA(digits[rest][argument_number]); argument_number ++;}
  }

  void LCD_SEND_TEXT (uint8_t x, uint8_t y, const char* text) // location x, location y, data
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  char a, c;
	  unsigned char argument_number;




	  address_x=0x80;             // stała wartość którš LCD interpretuje jako adres
	  address_y=0x40;             // jak wyżej
	  address_x=address_x+x;
	  address_y=address_y+y;

	  LCD_CMD(address_x);   // location of cursor
	  LCD_CMD(address_y);   //

	  c = *text;

	  while(c)
	  {
		  argument_number = 0;
		  for(a = 5; a > 0; a--) {LCD_DATA(font_ASCII[*text - 32][argument_number]); argument_number ++; }
		  LCD_DATA(0x00);
		  text ++;
		  c = *text;
	  }

  }

  void send_time_data (void)
  {
	  uint8_t week_day;


	  RTC_TimeTypeDef time;     // varriable of time
	  RTC_DateTypeDef date;     // varriable of date
	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

	  week_day = date.WeekDay;



	  LCD_SEND_TEXT(0, 0, "Today:");


	  switch (week_day)
	  {
	  case 1: LCD_SEND_TEXT(0, 1, "Monday");
	  break;
	  case 2: LCD_SEND_TEXT(0, 1, "Tuesday");
	  break;
	  case 3: LCD_SEND_TEXT(0, 1, "Wednesday");
	  break;
	  case 4: LCD_SEND_TEXT(0, 1, "Thursday");
	  break;
	  case 5: LCD_SEND_TEXT(0, 1, "Friday");
	  break;
	  case 6: LCD_SEND_TEXT(0, 1, "Saturday");
	  break;
	  case 7: LCD_SEND_TEXT(0, 1, "Sunday");
	  break;
	  }

	  LCD_SEND_DIGIT(0, 2, time.Hours);
	  LCD_DATA(0x00);
	  LCD_DATA(0x44);
	  LCD_SEND_DIGIT(15, 2, time.Minutes);
	  LCD_DATA(0x00);
	  LCD_DATA(0x44);
	  LCD_SEND_DIGIT(29, 2, time.Seconds);

	  LCD_SEND_DIGIT(0, 3, date.Date);
	  LCD_DATA(0x00);
	  LCD_DATA(0x80);

	  LCD_SEND_DIGIT(15, 3, date.Month);
	  LCD_DATA(0x00);
	  LCD_DATA(0x80);

	  LCD_SEND_DIGIT(30, 3, 20);
	  LCD_SEND_DIGIT(42, 3, date.Year);







  }




  void encoder_rotation_direction (void)
   {
 	  int tim1_value = htim1.Instance->CNT;
 	  static int last_value = 0;
 	  signed char direction;

 	  direction = last_value - tim1_value;
 	  last_value = tim1_value;

 	  if(direction > 0) LCD_SEND_PICTURE(40, 3, 8, 8, enter);
 	  if(direction < 0) LCD_SEND_PICTURE(40, 3, 8, 8, escape);


   }





  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



	  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);   // encoder start

	  LCD_ON();
	  LCD_CLEAR();
	  LCD_SEND_PICTURE(0, 0, 84, 48, intro);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LCD_LIGHT_Pin, GPIO_PIN_RESET);

	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_Delay(1999);
	  LCD_CLEAR();
	  HAL_Delay(999);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET);

	  while (1)
	  {


		  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)
		  	  {
			  	  LCD_SEND_PICTURE(0, 0, 8, 8, escape);
			  	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET);
		  	  }
		  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)
		  	  {

				  LCD_SEND_PICTURE(0, 0, 8, 8, enter);
		  	  }
		  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_SET) {HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);}


		  int timer1 = htim1.Instance->CNT;
		  LCD_SEND_DIGIT(40, 5, timer1);   // value encoder


		  encoder_rotation_direction();



		  send_time_data();
		  HAL_Delay(99);










	  }


  }
  /* USER CODE END 3 */
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 12;
  sTime.Minutes = 10;
  sTime.Seconds = 23;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_SEPTEMBER;
  sDate.Date = 28;
  sDate.Year = 22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 15;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_Pin|LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin
                          |LCD_LIGHT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RELAY_Pin */
  GPIO_InitStruct.Pin = RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELAY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin LCD_RST_Pin LCD_CE_Pin LCD_DC_Pin
                           LCD_LIGHT_Pin */
  GPIO_InitStruct.Pin = LED_Pin|LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin
                          |LCD_LIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ESCAPE_Pin ENTER_Pin */
  GPIO_InitStruct.Pin = ESCAPE_Pin|ENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
