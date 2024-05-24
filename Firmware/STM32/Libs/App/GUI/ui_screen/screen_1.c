/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Icon/icon.h"
#include "graphics.h"

field_bit_screen1_t bit_map_screen_1;

/**
 * The function `screen_1` clears the screen and displays different icons based on the values of
 * corresponding bit_map_screen_1 flags.
 */
void screen_1(EventBits_t uxBits)
{
	if (bit_map_screen_1.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_1.screen = 0;
	}
	if (bit_map_screen_1.wifi == 1)
	{
		GraphicsColourBitmap(99, 51, 41, 39, icon_wifi);
		bit_map_screen_1.wifi = 0;
	}
	if (bit_map_screen_1.home == 1)
	{
		GraphicsColourBitmap(90, 129, 50, 48, icon_home);
		bit_map_screen_1.home = 0;
	}
	if (bit_map_screen_1.MQTT == 1)
	{
		GraphicsColourBitmap(94, 208, 46, 44, icon_MQTT);
		bit_map_screen_1.MQTT = 0;
	}

	if (bit_map_screen_5.MQTT_Connected == 1)
	{
		GraphicsFilledRectangle(145, 209, 5, 5, GREEN);
	}
	else
	{
		GraphicsFilledRectangle(145, 209, 5, 5, RED);
	}

	if (bit_map_screen_2.WIFI_Connected == 1)
	{
		GraphicsFilledRectangle(140, 52, 5, 5, GREEN);
	}
	else
	{
		GraphicsFilledRectangle(140, 52, 5, 5, RED);
	}
}
