/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Keypad/keypad_wifi.h"
#include "graphics.h"
#include "icon.h"
#include "main.h"
#include <string.h>

/**********************
 *     VARIABLES
 **********************/

extern EventGroupHandle_t event_uart_rx;

field_bit_screen3_t bit_map_screen_3;
char text[18];
EventBits_t bitsScreen3;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void screen_3(EventBits_t uxBits)
{
	if (bit_map_screen_3.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_3.screen = 0;
	}

	if (bit_map_screen_3.frame == 1)
	{
		GraphicsRoundedRectangle(35, 82, 170, 30, 5, BLACK);
		bit_map_screen_3.frame = 0;
	}

	if (bit_map_screen_3.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_3.ret = 0;
	}

	if (bit_map_screen_3.text == 1)
	{
		GraphicsFilledRectangle(45, 50, 200, 20, WHITE);
		GraphicsLargeString(45, 56, text, BLACK);
		bit_map_screen_3.text = 0;
	}

	if (bit_map_screen_3.key == 1)
	{
		object_keypad_Wifi();
		bit_map_screen_3.key = 0;
	}

	bitsScreen3 = uxBits;
	
	if (uxBits & CONNECT_WIFI_UNSUCCESSFUL_BIT)
	{
		strcpy(text, "re-enter password");
		bit_map_screen_3.text = 1;
	}
}
