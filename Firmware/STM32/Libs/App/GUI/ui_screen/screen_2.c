/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/

#define MAX_LENGTH_SSID 13
#define SSID_IN_PAGE 5


/**********************
 *     VARIABLES
 **********************/

field_bit_screen2_t bit_map_screen_2;
uint8_t state_wifi = 0;
uint8_t numSSIDofPage = 0;
uint8_t update_state = 0;

char ssid1[32];
char ssid2[32];
char ssid3[32];
char ssid4[32];
char ssid5[32];

char ssid_connect[32];

/**********************
 *  STATIC VARIABLES
 **********************/

char bufferEffectScreen2[174];

/**********************
 *  EXTERN VARIABLES
 **********************/

extern char buffer_uart_rx[RX_BUFFER_SIZE + 1];
extern EventGroupHandle_t event_uart_rx;
extern EventGroupHandle_t event_uart_tx;
extern TimerHandle_t timer_request_scan_wifi;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `screen_2` handles various graphical elements and logic related to Wi-Fi connectivity
 * on a screen.
 *
 * @param uxBits The `uxBits` parameter in the `screen_2` function is a bitmask representing various
 * event bits that have been set. The function checks these bits to determine which actions to take
 * based on the events that have occurred. Each bit in the `uxBits` parameter corresponds to a specific
 * event or
 */
void screen_2(EventBits_t uxBits)
{
	 if (uxBits & SCAN_WIFI_BIT)
	 {
		update_state = 1;
	 	char *token;
	 	uint8_t position = 0;
	 	numSSIDofPage = 1;
	 	bit_map_screen_2.WIFI1 = 0;
	 	bit_map_screen_2.WIFI2 = 0;
	 	bit_map_screen_2.WIFI3 = 0;
	 	bit_map_screen_2.WIFI4 = 0;
	 	bit_map_screen_2.WIFI5 = 0;

        memset((void *)ssid1, '\0', sizeof(ssid1));
        memset((void *)ssid2, '\0', sizeof(ssid2));
        memset((void *)ssid3, '\0', sizeof(ssid3));
        memset((void *)ssid4, '\0', sizeof(ssid4));
        memset((void *)ssid5, '\0', sizeof(ssid5));

	 	strcpy(bufferEffectScreen2, (char *)buffer_uart_rx);
	 	token = strtok(bufferEffectScreen2, "\r");
	 	while (token != NULL)
	 	{
	 		if (position == (numSSIDofPage + 2)) break;
	 		if (position == 6)
	 		{
	 			bit_map_screen_2.WIFI5 = 1;
	 			strcpy(ssid5, token);
	 		}
			
	 		else if (position == 5)
	 		{
	 			bit_map_screen_2.WIFI4 = 1;
	 			strcpy(ssid4, token);
	 		}
	 		else if (position == 4)
	 		{
	 			bit_map_screen_2.WIFI3 = 1;
	 			strcpy(ssid3, token);
	 		}
	 		else if (position == 3)
	 		{
	 			bit_map_screen_2.WIFI2 = 1;
	 			strcpy(ssid2, token);
	 		}
	 		else if (position == 2)
	 		{
	 			bit_map_screen_2.WIFI1 = 1;
	 			strcpy(ssid1, token);
	 		}
	 		else if (position == 1)
	 		{
	 			numSSIDofPage = *token - 48;
	 		}
	 		else if (position == 0)
	 		{
	 			if (memcmp(token, "START", strlen(token) + 1) == 0) bit_map_screen_2.TAB_PAGE = 0;
	 			else if (memcmp(token, "MID", strlen(token) + 1) == 0) bit_map_screen_2.TAB_PAGE = 1;
	 			else if (memcmp(token, "END", strlen(token) + 1) == 0) bit_map_screen_2.TAB_PAGE = 2;
	 		}
			
	 		position++;
			
	 		token = strtok(NULL, "\r");
	 	}

		if (bit_map_screen_2.WIFI1 == 1)
		{
			GraphicsFilledRectangle(15, 110, 200, 25, BLACK);
			GraphicsLargeString(20, 115, ssid1, WHITE); // in ssid 1
		}
		else
			GraphicsFilledRectangle(15, 110, 200, 25, WHITE);

		if (bit_map_screen_2.WIFI2 == 1)
		{
			GraphicsFilledRectangle(15, 145, 200, 25, BLACK);
			GraphicsLargeString(20, 150, ssid2, WHITE); // in ssid 2
		}
		else
			GraphicsFilledRectangle(15, 145, 200, 25, WHITE);

		if (bit_map_screen_2.WIFI3 == 1)
		{
			GraphicsFilledRectangle(15, 180, 200, 25, BLACK);
			GraphicsLargeString(20, 185, ssid3, WHITE); // in ssid 3
		}
		else
			GraphicsFilledRectangle(15, 180, 200, 25, WHITE);

		if (bit_map_screen_2.WIFI4 == 1)
		{
			GraphicsFilledRectangle(15, 215, 200, 25, BLACK);
			GraphicsLargeString(20, 220, ssid4, WHITE); // in ssid 4
		}
		else
			GraphicsFilledRectangle(15, 215, 200, 25, WHITE);

		if (bit_map_screen_2.WIFI5 == 1)
		{
			GraphicsFilledRectangle(15, 250, 200, 25, BLACK);
			GraphicsLargeString(20, 255, ssid5, WHITE); // in ssid 5
		}
		else
			GraphicsFilledRectangle(15, 250, 200, 25, WHITE);

		
		if (bit_map_screen_2.TAB_PAGE == 0)
		{
			GraphicsLargeString(167, 298, "NEXT->", BLACK);
			GraphicsLargeString(11, 298, "<-BACK", WHITE);
		}
		else if (bit_map_screen_2.TAB_PAGE == 1)
		{
			GraphicsLargeString(167, 298, "NEXT->", BLACK);
			GraphicsLargeString(11, 298, "<-BACK", BLACK);
		}
		else if (bit_map_screen_2.TAB_PAGE == 2)
		{
			GraphicsLargeString(167, 298, "NEXT->", WHITE);
			GraphicsLargeString(11, 298, "<-BACK", BLACK);
		}

		if (bit_map_screen_2.WIFI_Connected == 1)
		{
			GraphicsFilledRectangle(15, 54, 200, 25, BLACK);
			GraphicsLargeString(32, 57, ssid_connect, WHITE); // in ssid connected
		}
		else
			GraphicsFilledRectangle(15, 54, 200, 25, WHITE);
	 }

	if (bit_map_screen_2.screen == 1)
	{
		GraphicsClear(WHITE);
		GraphicsRoundedRectangle(35, 54, 170, 25, 5, BLACK);
		bit_map_screen_2.screen = 0;
	}

	if (bit_map_screen_2.ret == 1)
	{
		GraphicsColourBitmap(5, 7, 28, 23, icon_return);
		bit_map_screen_2.ret = 0;
	}

	if (bit_map_screen_2.text1 == 1)
	{
		GraphicsLargeString(165, 34, "Wi-Fi", BLACK);
		bit_map_screen_2.text1 = 0;
	}

	if (bit_map_screen_2.text2 == 1)
	{
		GraphicsLargeString(5, 90, "Wi-Fi Network", BLACK);
		GraphicsHline(0, 240, 90, BLACK);
		bit_map_screen_2.text2 = 0;
	}

	if (bit_map_screen_2.on_off_wifi == 1)
	{
		if (state_wifi == 1)
		{
			GraphicsFilledRectangle(165, 8, 40, 25, GREEN); // green if wifi on
		}
		else
		{
			GraphicsFilledRectangle(165, 8, 40, 25, RED); // red if wifi off
		}
	}

	if (uxBits & CONNECT_WIFI_SUCCESSFUL_BIT)
	{
		strcpy(ssid_connect,buffer_uart_rx);
	}
	
	if (state_wifi == 0)
	{
		bit_map_screen_2.WIFI_Connected = 0;
		GraphicsLargeString(167, 298, "NEXT->", WHITE);
		GraphicsLargeString(11, 298, "<-BACK", WHITE);
		GraphicsFilledRectangle(15, 54, 200, 25, WHITE); 
		GraphicsFilledRectangle(15, 110, 200, 25, WHITE);
		GraphicsFilledRectangle(15, 145, 200, 25, WHITE);
		GraphicsFilledRectangle(15, 180, 200, 25, WHITE);
		GraphicsFilledRectangle(15, 215, 200, 25, WHITE);
		GraphicsFilledRectangle(15, 250, 200, 25, WHITE);
	}
}
