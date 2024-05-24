/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "main.h"
#include <string.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "event_groups.h"

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;
extern uint8_t flag_is_touch;

extern uint8_t buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern uint8_t buffer_uart_tx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_tx;
extern uint8_t state_wifi;

extern TimerHandle_t timer_request_scan_wifi;

extern char text[18];
extern char ssid[32];

extern char ssid1[32];
extern char ssid2[32];
extern char ssid3[32];
extern char ssid4[32];
extern char ssid5[32];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void check_event_screen_2(screen_state_t *screen)
{
	touch_icon_screen2_t touch = check_event_icon_screen2(x, y);
	switch (touch)
	{
	case ICON_RETURN:
		if (flag_is_touch == 0)
		{
			bit_map_screen_1.screen = 1;
			bit_map_screen_1.wifi = 1;
			bit_map_screen_1.home = 1;
			bit_map_screen_1.MQTT = 1;
			*screen = SCREEN_START;
			xTimerStop(timer_request_scan_wifi, 0);

			x = RESET_COORDINATE;
			y = RESET_COORDINATE;
			flag_is_touch = 1;
		}
		break;
	case ON_OFF_WIFI:
		if (flag_is_touch == 0)
		{
			bit_map_screen_2.on_off_wifi = 1; // draw button on off wifi

			if(state_wifi == 1)	state_wifi = 0;
			else state_wifi = 1;

			if (state_wifi == 1)
			{
				xEventGroupSetBits(event_uart_tx, ON_WIFI_BIT);
			 	xTimerStart(timer_request_scan_wifi, 0);
			}
			else
			{
				xTimerStop(timer_request_scan_wifi, 0);
				xEventGroupSetBits(event_uart_tx, OFF_WIFI_BIT);
			}

			x = RESET_COORDINATE;
			y = RESET_COORDINATE;
			flag_is_touch = 1;
		}
		break;
	case NEXT:
		if (flag_is_touch == 0)
		{
			xEventGroupSetBits(event_uart_tx, NEXT_PAGE_BIT);
			xTimerReset(timer_request_scan_wifi, 0);

			x = RESET_COORDINATE;
			y = RESET_COORDINATE;

			flag_is_touch = 1;
		}
		break;
	case BACK:
		if (flag_is_touch == 0)
		{
			xEventGroupSetBits(event_uart_tx, BACK_PAGE_BIT);
			xTimerReset(timer_request_scan_wifi, 0);

			x = RESET_COORDINATE;
			y = RESET_COORDINATE;

			flag_is_touch = 1;
		}
		break;
	case WIFI1:
	case WIFI2:
	case WIFI3:
	case WIFI4:
	case WIFI5:
		if (flag_is_touch == 0)
		{
			xTimerStop(timer_request_scan_wifi, 0);
			strcpy(text, "enter password");
			bit_map_screen_3.screen = 1;
			bit_map_screen_3.ret = 1;
			bit_map_screen_3.text = 1;
			bit_map_screen_3.frame = 1;
			bit_map_screen_3.key =1;
			*screen = SCREEN_KEYPAD;
		
			if (touch == WIFI1)			strcpy(ssid, ssid1);
			else if (touch == WIFI2)	strcpy(ssid, ssid2);
			else if (touch == WIFI3)	strcpy(ssid, ssid3);
			else if (touch == WIFI4)	strcpy(ssid, ssid4);
			else if (touch == WIFI5)	strcpy(ssid, ssid5);

			x = RESET_COORDINATE;
			y = RESET_COORDINATE;
			flag_is_touch = 1;
		}
		break;
	case NO_TOUCH_ICON_SC2:
		flag_is_touch = 0;
		return;
	}
}
