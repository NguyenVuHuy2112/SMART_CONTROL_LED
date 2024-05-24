/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "Keypad/keypad_wifi.h"
#include <string.h>
#include "main.h"
#include "screen.h"

/*********************
 *      DEFINES
 *********************/
#define START_COORDINATE		37
#define OFFSET_X 				15
#define OFFSET_Y 				90
#define MIN_LENGTH_PASSWORD		8

/**********************
 *  STATIC VARIABLES
 **********************/

static const char character_key[12] = { '1', '2', '3',
									   '4', '5', '6', '7',
									   '8', '9', 'x','0', 'v'};
static uint8_t password_pos = 0;
static char password[9];

/**********************
 *  EXTERN VARIABLES
 **********************/

extern int16_t x;
extern int16_t y;
extern uint8_t flag_is_touch;

extern char buffer_uart_tx[RX_BUFFER_SIZE + 1];
extern char buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;

extern TimerHandle_t timer_request_scan_wifi;

extern char text[18];
extern EventBits_t bitsScreen3;

extern char ssid_connect[32];

/**********************
 *     VARIABLES
 **********************/

char ssid[32];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void check_event_screen_3(screen_state_t *screen)
{
	static uint16_t x_coordinate = START_COORDINATE; /*variable containing location*/

	key_character_t key = check_event_keypad_Wifi(x, y); /*checking touch position of each element in the keyboard */
	if (key != NO_TOUCH)
	{
		if (flag_is_touch == 0)
		{
			if (key == NUM_x)
			{
				memset(password, '\0', sizeof(password));
				bit_map_screen_3.frame = 1;
				password_pos = 0;
				x_coordinate = START_COORDINATE;
			}

			else if (key == ICON_RETURN) /*return to screen_2*/
			{
				memset(password, '\0', sizeof(password));
				password_pos = 0;
				x_coordinate = START_COORDINATE;

				bit_map_screen_2.screen = 1;
				bit_map_screen_2.ret = 1;
				bit_map_screen_2.WIFI_Connected = 1;
				bit_map_screen_2.on_off_wifi = 1;
				bit_map_screen_2.text1 = 1;
				bit_map_screen_2.text2 = 1;
				xEventGroupSetBits(event_uart_tx, SCAN_WIFI_BIT);
				// bit_map_screen_2.WIFI1 = 1;
				// bit_map_screen_2.WIFI2 = 1;
				// bit_map_screen_2.WIFI3 = 1;
				// bit_map_screen_2.WIFI4 = 1;
				// bit_map_screen_2.WIFI5 = 1;

				bit_map_screen_2.TAB_PAGE = 0;

				*screen = SCREEN_WIFI;
			}

			else if (key == NUM_v) /*If you click check, check to see if the entered password is sufficient.
									If so, write it to Queue to check*/
			{
				if (password_pos >= (MIN_LENGTH_PASSWORD - 1))
				{
					password_pos = 0;
					x_coordinate = START_COORDINATE;
					sprintf((char *)buffer_uart_tx, "%s\r%s", ssid, password);
					xEventGroupSetBits(event_uart_tx, CONNECT_WIFI_BIT);
					memset(password, '\0', sizeof(password));
				}
				else
				{
					bit_map_screen_3.text = 1;
					strcpy(text, "Min 8 number");
				}
			}

			else /*touch keypad to entering password*/
			{
				password[password_pos] = character_key[key];
				GraphicsLargeCharacter(x_coordinate, OFFSET_Y,
									character_key[key], WHITE);
				password_pos++;
				x_coordinate += OFFSET_X;

			}
			flag_is_touch = 1;
		}
		x = RESET_COORDINATE;
		y = RESET_COORDINATE;
	}
	else
	{
		flag_is_touch = 0;
	}

	if (bitsScreen3 & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		sprintf(ssid_connect, "%s", ssid);
//		strcpy(ssid_connect,ssid);
		xTimerStart(timer_request_scan_wifi, 0);
		bit_map_screen_2.screen = 1;
		bit_map_screen_2.ret = 1;
		bit_map_screen_2.on_off_wifi = 1;
		bit_map_screen_2.WIFI_Connected = 1;
		bit_map_screen_2.text1 = 1;
		bit_map_screen_2.text2 = 1;
		*screen = SCREEN_WIFI;
	}
}
