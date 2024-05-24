/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "Keypad/keypad_MQTT.h"
#include "graphics.h"
#include "icon.h"
#include "main.h"
#include "string.h"

/**********************
 *     VARIABLES
 **********************/

extern EventGroupHandle_t event_uart_rx;

field_bit_screen5_t bit_map_screen_5;
char text_sc5[14];
EventBits_t bitsScreen5;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void screen_5(EventBits_t uxBits)
{
	if (bit_map_screen_5.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_5.screen = 0;
	}

	if (bit_map_screen_5.frame == 1)
	{
		GraphicsRoundedRectangle(95, 82, 60, 30, 5, BLACK);
		bit_map_screen_5.frame = 0;
	}

	if (bit_map_screen_5.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_5.ret = 0;
	}

	if (bit_map_screen_5.text == 1)
	{
		GraphicsFilledRectangle(80, 63, 90, 15, WHITE);
		GraphicsLargeString(82, 63, text_sc5, BLACK);
		bit_map_screen_5.text = 0;
	}

	if (bit_map_screen_5.key == 1)
	{
		object_Keypad_MQTT();
		bit_map_screen_5.key = 0;
	}

	bitsScreen5 = uxBits;

	if (uxBits & CONNECT_MQTT_UNSUCCESSFUL_BIT)
	{
		strcpy(text_sc5, "re-enter MQTT");
		bit_map_screen_5.text = 1;
	}

	if (uxBits & REFUSE_CONNECT_MQTT_BIT)
	{
		strcpy(text_sc5, "Connect Wifi!");
		bit_map_screen_5.text = 1;
	}
	
}
