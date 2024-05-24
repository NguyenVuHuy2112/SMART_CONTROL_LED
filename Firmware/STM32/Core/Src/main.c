/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "delay_timer.h"

#include "graphics.h"
#include "calibrate_touch.h"
#include "ili9341.h"
#include "touch.h"
#include "screen.h"
#include "event.h"

#include "read_adc.h"
#include "cal_temperature.h"
#include "illuminance.h"
#include "auto_led.h"


#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"
#include "timers.h"


#include <string.h>

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
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/**********************
 *  EXTERN VARIABLES
 **********************/

extern float Temperature;
extern int16_t Ev;
extern control_led_t led_state;
extern control_auto_t auto_state;
extern uint8_t MQTT[15];
extern uint8_t MQTT_pos;

/**********************
 *  STATIC VARIABLES
 **********************/
// Task Handle
static TaskHandle_t uart_rx_task;
static TaskHandle_t uart_tx_task;
static TaskHandle_t control_led_task;
static TaskHandle_t screen_task;

static screen_state_t screen_current = SCREEN_START;
static TimerHandle_t timer_refresh_display;

/**********************
 *  GLOBAL VARIABLES
 **********************/

int16_t x;
int16_t y;
uint8_t flag_is_touch = 0;

char buffer_uart_rx[RX_BUFFER_SIZE + 1];
char buffer_uart_tx[RX_BUFFER_SIZE + 1];

// QueueHandle
QueueHandle_t queue_control_led = NULL;
QueueHandle_t queue_control_auto = NULL;

// EventGroup Handle
EventGroupHandle_t event_uart_tx;
EventGroupHandle_t event_uart_rx;

// Timer Handle
TimerHandle_t timer_request_scan_wifi;
TimerHandle_t timer_wait_off_screen;
TimerHandle_t timer_request_mqtt_Pub;

static control_led_t check_state_led = LED_OFF;
static control_auto_t check_state_auto = AUTO_OFF;

 int16_t lux = 0;
 int16_t value_adc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_UART4_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

static void Screen_Task(void *pvParameters);
static void UartTx_Task(void *pvParameters);
static void UartRx_Task(void *pvParameters);
static void ControlLed_Task(void *pvParameters);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void processingDataMQTTPublish()
{
	  char state_led[4], state_auto[4];
	  if (check_state_led == LED_ON) strcpy(state_led, "ON");
	  else strcpy(state_led, "OFF");

	  if (check_state_auto == AUTO_ON) strcpy(state_auto, "ON");
	  else strcpy(state_auto, "OFF");
		temperature_sensor_enable(value_adc, &hadc1);
		Temperature = calculate_temperature(value_adc);
		float volt = voltage_adc();
		float ev_before = illuminance_adc(volt);
	  lux = adjust_Ev(ev_before);

	  sprintf((char *)buffer_uart_tx, "%s\r%s\r%d\r%.2f\r", state_led, state_auto, lux, Temperature);
}

void timerRequestScanWifi(TimerHandle_t xTimer)
{
	xEventGroupSetBits(event_uart_tx, ON_WIFI_BIT);
}

void timerRefreshDisplayCb(TimerHandle_t xTimer)
{
	xEventGroupSetBits(event_uart_rx, REFRESH_DISPLAY_BIT);
}

void vBacklightTimerCb(TimerHandle_t xTimer)
{
	screen_current = SCREEN_OFF;
	GraphicsClear(BLACK);
	xTimerStop(timer_request_scan_wifi, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
}

void timerRequestmqttPub(TimerHandle_t xTimer)
{
	processingDataMQTTPublish();
  xEventGroupSetBits(event_uart_tx, MQTT_PUBLISH_BIT);
}
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */


  // Init driver

  GraphicsInit();
  TouchCalibrate();
  UARTConfig(&huart4, RX_BUFFER_SIZE * 2);

  // Init application

  bit_map_screen_1.screen = 1;
  bit_map_screen_1.wifi = 1;
  bit_map_screen_1.home = 1;
  bit_map_screen_1.MQTT = 1;

  bit_map_screen_5.MQTT_Connected = 0;

  bit_map_screen_2.WIFI_Connected = 0;

  // init service
	queue_control_led = xQueueCreate(1, sizeof(control_led_t));
	queue_control_auto = xQueueCreate(1, sizeof(control_auto_t));

	event_uart_tx = xEventGroupCreate();
	event_uart_rx = xEventGroupCreate();

	timer_request_scan_wifi = xTimerCreate("timer scan", TIME_REQUEST_SCAN, pdTRUE, (void *)0, timerRequestScanWifi);
	timer_refresh_display = xTimerCreate("timer refresh", TIME_REFRESH_DISPLAY, pdTRUE, 0, timerRefreshDisplayCb);
	timer_wait_off_screen = xTimerCreate("timer wait", TIME_WAIT, pdFALSE, (void *)0, vBacklightTimerCb);
	timer_request_mqtt_Pub = xTimerCreate("timer read", TIME_READ, pdTRUE, (void *)0, timerRequestmqttPub);

  // init task

	xTaskCreate(Screen_Task, "Screen_Task", configMINIMAL_STACK_SIZE * 16, NULL, 4, &screen_task);
	xTaskCreate(UartTx_Task, "Transmit_Task", configMINIMAL_STACK_SIZE, NULL, 2, &uart_tx_task);
	xTaskCreate(UartRx_Task, "Receive_Task", configMINIMAL_STACK_SIZE * 3, NULL, 2, &uart_rx_task);
	xTaskCreate(ControlLed_Task, "led_Task", configMINIMAL_STACK_SIZE, NULL, 3, &control_led_task);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//
  vTaskStartScheduler();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL15;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

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
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

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
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 59;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
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
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TFT_DC_RS_Pin|TFT_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TFT_DC_RS_Pin TFT_RESET_Pin */
  GPIO_InitStruct.Pin = TFT_DC_RS_Pin|TFT_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void Screen_Task(void *pvParameters)
{
	xTimerStart(timer_refresh_display, 0);
	xTimerStart(timer_wait_off_screen, 0);
  while (1)
  {
    // state machine ui_screen (choose ui screen)
    EventBits_t uxBits = xEventGroupWaitBits(event_uart_rx, SCAN_WIFI_BIT |
              CONNECT_WIFI_SUCCESSFUL_BIT | 
              CONNECT_WIFI_UNSUCCESSFUL_BIT | 
              REFUSE_CONNECT_MQTT_BIT | 
              CONNECT_MQTT_SUCCESSFUL_BIT | 
              CONNECT_MQTT_UNSUCCESSFUL_BIT | 
              REFRESH_DISPLAY_BIT,
			  pdTRUE, pdFALSE, portMAX_DELAY);
    if ((uxBits & CONNECT_WIFI_UNSUCCESSFUL_BIT) == CONNECT_WIFI_UNSUCCESSFUL_BIT)
    {
    	bit_map_screen_2.WIFI_Connected = 0;
    }

	if (uxBits & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		bit_map_screen_2.WIFI_Connected = 1;
	}
    if (uxBits != 0)
    {
		float volt = voltage_adc();
		float ev_before = illuminance_adc(volt);
		lux = adjust_Ev(ev_before);

		temperature_sensor_enable(value_adc, &hadc1);
		Temperature = calculate_temperature(value_adc);
      if (flag_is_touch == 1)
      {
        xTimerReset( timer_wait_off_screen, 0 );   // reset Timer
      }
      TouchGetCalibratedPoint(&x, &y);
		switch (screen_current)
		{
		  case SCREEN_START:
			check_event_screen_1(&screen_current);
			screen_1(uxBits);
			break;

		  case SCREEN_WIFI:
			check_event_screen_2(&screen_current);
			screen_2(uxBits);
			break;

		  case SCREEN_KEYPAD:
			check_event_screen_3(&screen_current);
			screen_3(uxBits);
			break;

		  case SCREEN_MAIN:
			check_event_screen_4(&screen_current);
			screen_4(uxBits);
			break;

		  case SCREEN_MQTT:
			check_event_screen_5(&screen_current);
			screen_5(uxBits);
			break;

		  case SCREEN_OFF:
			check_event_screen_6(&screen_current);
			break;
		}
    }
  }
}

static void transmitdata(uart_tx_heading_t heading, char *data)
{
	UARTWrite((char *)&heading, 1);
	UARTWrite(data, strlen(data));
	UARTWrite("\n", 1);
}



static void UartTx_Task(void *pvParameters)
{
	memset(buffer_uart_tx, '\0', sizeof(buffer_uart_tx));
	while (1)
	{
		EventBits_t uxBits = xEventGroupWaitBits(event_uart_tx,
												 ON_WIFI_BIT |
												 OFF_WIFI_BIT |
												 NEXT_PAGE_BIT |
												 BACK_PAGE_BIT |
												 CONNECT_WIFI_BIT |
												 CONNECT_MQTT_BIT |
												 MQTT_PUBLISH_BIT,
												 pdTRUE, pdFALSE,
												 portMAX_DELAY);
		if (uxBits & ON_WIFI_BIT)
		{
		  sprintf((char *)buffer_uart_tx, "%s", "ON");
		  transmitdata(HEADING_WIFI, (char *)buffer_uart_tx);
		}

		if (uxBits & OFF_WIFI_BIT)
		{
		  sprintf((char *)buffer_uart_tx, "%s", "OFF");
		  transmitdata(HEADING_WIFI, (char *)buffer_uart_tx);
		}

		if (uxBits & NEXT_PAGE_BIT)
		{
		  sprintf((char *)buffer_uart_tx, "%s", "NEXT");
		  transmitdata(HEADING_TAB_PAGE, (char *)buffer_uart_tx);
		}

		if (uxBits & BACK_PAGE_BIT)
		{
		  sprintf((char *)buffer_uart_tx, "%s", "BACK");
		  transmitdata(HEADING_TAB_PAGE, (char *)buffer_uart_tx);
		}

		if (uxBits & CONNECT_WIFI_BIT) // send ssid-pass from event_screen_3
		{
		  transmitdata(HEADING_CONNECT_WIFI, (char *)buffer_uart_tx);
		}

		if (uxBits & CONNECT_MQTT_BIT) // send ip of mqtt from event_screen_5
		{
		  transmitdata(HEADING_CONNECT_MQTT, (char *)buffer_uart_tx);
		}

		if (uxBits & MQTT_PUBLISH_BIT)
		{

		  transmitdata(HEADING_MQTT_PUBLISH, (char *)buffer_uart_tx);
		}

		memset(buffer_uart_tx, '\0', sizeof(buffer_uart_tx));
	}
}

static void UartRx_Task(void *pvParameters)
{
	uint8_t buffer_temp[RX_BUFFER_SIZE + 1];
	uint16_t pos_buffer_uart_rx = 0;
	uint8_t enable_bit = 0;
	int8_t data_heading = -1;
	uint16_t i;

    memset((void *)buffer_uart_rx, '\0', sizeof(buffer_uart_rx));

	while (1)
	{
		uint32_t rxBytes = UARTRead((char *)buffer_temp);

		if (rxBytes > 0)
		{
			for (i = 0; i < rxBytes; i++)
			{
				buffer_uart_rx[pos_buffer_uart_rx] = buffer_temp[i];
				if (buffer_uart_rx[pos_buffer_uart_rx] == '\n')
				{
					enable_bit = 1;
					buffer_uart_rx[pos_buffer_uart_rx] = '\0';
					pos_buffer_uart_rx = 0;
					break;
				}
				pos_buffer_uart_rx++;
			}

			if (enable_bit == 0)
			{
				continue;
			}

			xTimerStop(timer_refresh_display, 0);

			data_heading = buffer_uart_rx[0];
			uint8_t size_buffer = strlen(buffer_uart_rx);

			for (i = 0; i < size_buffer; i++)
			{
				buffer_uart_rx[i] = buffer_uart_rx[i + 1];
			}

			switch (data_heading)
			{
				case SCAN_WIFI_BIT:
					xEventGroupSetBits(event_uart_rx, SCAN_WIFI_BIT);
					break;

				case HEADING_RECEIVE_CONNECT_WIFI:
					if (memcmp(buffer_uart_rx, "FAILED", strlen(buffer_uart_rx) + 1) == 0)
					{
					  xEventGroupSetBits(event_uart_rx, CONNECT_WIFI_UNSUCCESSFUL_BIT);
					}
					else
					{
					  xEventGroupSetBits(event_uart_rx, CONNECT_WIFI_SUCCESSFUL_BIT);
					}
					break;

				case HEADING_RECEIVE_CONNECT_MQTT:
					if (memcmp(buffer_uart_rx, "TRUE", strlen(buffer_uart_rx) + 1) == 0)
					{
						xTimerStart(timer_request_mqtt_Pub, 0);
					  xEventGroupSetBits(event_uart_rx, CONNECT_MQTT_SUCCESSFUL_BIT);
					}
					else if (memcmp(buffer_uart_rx, "FAILED", strlen(buffer_uart_rx) + 1) == 0)
					{
						xTimerStop(timer_request_mqtt_Pub, 0);
					  xEventGroupSetBits(event_uart_rx, CONNECT_MQTT_UNSUCCESSFUL_BIT);
					}
					else if (memcmp(buffer_uart_rx, "REFUSE", strlen(buffer_uart_rx) + 1) == 0)
					{
						xTimerStop(timer_request_mqtt_Pub, 0);
					  xEventGroupSetBits(event_uart_rx, REFUSE_CONNECT_MQTT_BIT);
					}
					break;

				case HEADING_MQTT_SUBSCRIBE:
				    uint8_t arg_position = 0;

				    // cut string
				    char *temp_token = strtok(buffer_uart_rx, "\r");
				    while(temp_token != NULL)
				    {
				        if (arg_position == 0)
				        {
				        	if (memcmp(temp_token, "OFF", strlen(temp_token) + 1) == 0)
				        	{
				        		led_state = LED_OFF;
				        		xQueueSend(queue_control_led, &led_state, 0);
				        		bit_map_screen_4.OFF = 1; // if Button off
				        	}
				        	else
				        	{
				        		led_state = LED_ON;
				        		xQueueSend(queue_control_led, &led_state, 0);
				        		bit_map_screen_4.ON = 1; // if Button on
				        	}
				        }
				        else if (arg_position == 1)
				        {
				        	if (memcmp(temp_token, "OFF", strlen(temp_token) + 1) == 0)
				        	{
				        		auto_state = AUTO_OFF;
				        		xQueueSend(queue_control_auto, &auto_state, 0);
				        		bit_map_screen_4.off_auto = 1;
				        	}
				        	else
				        	{
				        		auto_state = AUTO_ON;
				        		xQueueSend(queue_control_auto, &auto_state, 0);
				        		bit_map_screen_4.on_auto = 1;
				        	}
				        }
				        arg_position++;

				        temp_token = strtok(NULL, "\r");
				    }
					xEventGroupSetBits(event_uart_rx, HEADING_MQTT_SUBSCRIBE);
					break;

				default:

					break;
			}

			memset((void *)buffer_uart_rx, '\0', sizeof(buffer_uart_rx));
			enable_bit = 0;
			xTimerReset(timer_refresh_display, 0);
		}
		vTaskDelay(50);
	  }
}

static void ControlLed_Task(void *pvParameters)
{
	BaseType_t ret;
	while (1)
	{
		ret = xQueueReceive(queue_control_auto, &check_state_auto, 1000);

		if (ret == pdPASS)
		{
			if (check_state_auto == AUTO_ON)
			{
				if(autocontrol_mode() == 1)
				{
					bit_map_screen_4.ON = 1;
					check_state_led = LED_ON;
					led_state = LED_ON;
				}
				else
				{
					bit_map_screen_4.OFF = 1;
					check_state_led = LED_OFF;
					led_state = LED_OFF;
				}
			}
		}

		ret = xQueueReceive(queue_control_led, &check_state_led, 1000);

		if (ret == pdPASS)
		{
			if (check_state_led == LED_ON)	turnOnLight();
			else turnOffLight();
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM8) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
