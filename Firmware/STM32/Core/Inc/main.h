/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "uartstdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TFT_DC_RS_Pin GPIO_PIN_2
#define TFT_DC_RS_GPIO_Port GPIOA
#define TFT_RESET_Pin GPIO_PIN_3
#define TFT_RESET_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define RX_BUFFER_SIZE 					          100

#define TIME_REQUEST_SCAN                 5000
#define TIME_REFRESH_DISPLAY              50
#define TIME_WAIT                         30000
#define TIME_READ						    5000

//coordinate screen
#define RESET_COORDINATE                  1000

// UART TX EVENT
#define ON_WIFI_BIT                       (1 << 0)
#define OFF_WIFI_BIT                      (1 << 1)
#define NEXT_PAGE_BIT						(1 << 2)
#define BACK_PAGE_BIT						(1 << 3)
#define CONNECT_WIFI_BIT                 	(1 << 4)
#define CONNECT_MQTT_BIT                 	(1 << 5)
#define MQTT_PUBLISH_BIT                 	(1 << 6)

// UART RX EVENT

#define SCAN_WIFI_BIT						(1 << 0)
#define CONNECT_WIFI_SUCCESSFUL_BIT       (1 << 2)
#define CONNECT_WIFI_UNSUCCESSFUL_BIT     (1 << 3)
#define REFUSE_CONNECT_MQTT_BIT           (1 << 4) // if haven't connect wifi
#define CONNECT_MQTT_SUCCESSFUL_BIT       (1 << 5)
#define CONNECT_MQTT_UNSUCCESSFUL_BIT     (1 << 6)
#define MQTT_SUBSCRIBE_BIT                (1 << 7)
#define REFRESH_DISPLAY_BIT               (1 << 9)

/**********************
 *      TYPEDEFS
 **********************/
typedef enum 
{
	HEADING_WIFI = 0x01,
	HEADING_TAB_PAGE,
	HEADING_CONNECT_WIFI,
	HEADING_CONNECT_MQTT,
	HEADING_MQTT_PUBLISH,
} uart_tx_heading_t;

typedef enum
{
	HEADING_RECEIVE_SCAN_WIFI = 0X01,
	HEADING_RECEIVE_CONNECT_WIFI,
	HEADING_RECEIVE_CONNECT_MQTT,
	HEADING_MQTT_SUBSCRIBE,
} uart_rx_heading_t;

typedef enum
{
	LED_OFF = 0,
	LED_ON,
} control_led_t;

typedef enum
{
	AUTO_OFF = 0,
	AUTO_ON,
} control_auto_t;

/*********************
 *   INLINE FUNCTION
 *********************/



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
