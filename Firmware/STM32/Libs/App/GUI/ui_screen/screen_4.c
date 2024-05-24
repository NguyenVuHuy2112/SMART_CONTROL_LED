/*********************
 *      INCLUDES
 *********************/

#include "screen.h"
#include "graphics.h"
#include "Icon/icon.h"
#include "illuminance.h"
#include "cal_temperature.h"
#include "read_adc.h"
#include "main.h"


field_bit_screen4_t bit_map_screen_4;
extern int16_t lux;
extern int16_t value_adc;
extern float Temperature;
extern ADC_HandleTypeDef hadc1;

void screen_4(EventBits_t uxBits)
{
	if (bit_map_screen_4.screen == 1)
	{
		GraphicsClear(WHITE);
		bit_map_screen_4.screen = 0;
	}

	if (bit_map_screen_4.ret == 1)
	{
		GraphicsColourBitmap(7, 8, 28, 23, icon_return);
		bit_map_screen_4.ret = 0;
	}

	if (bit_map_screen_4.control == 1)
	{
		GraphicsColourBitmap(91, 63, 59, 60, icon_button);
		bit_map_screen_4.control = 0;
	}

	if (bit_map_screen_4.automode == 1)
	{
		GraphicsColourBitmap(89, 176, 61, 60, icon_auto);
		bit_map_screen_4.automode = 0;
	}

	if(bit_map_screen_4.on_auto == 1)
	{
		GraphicsLargeString(94, 236, "AUTO", BLACK);
		bit_map_screen_4.on_auto = 0;
	}

	if(bit_map_screen_4.off_auto == 1)
	{
		GraphicsLargeString(94, 236, "AUTO", WHITE);
		bit_map_screen_4.off_auto = 0;
	}

	if (bit_map_screen_4.ON == 1)
	{
		GraphicsLargeString(108, 44, "ON", BLACK);
		GraphicsLargeString(108, 29, "OFF", WHITE);
		bit_map_screen_4.ON = 0;
	}

	if (bit_map_screen_4.OFF == 1)
	{
		GraphicsLargeString(108, 29, "OFF", BLACK);
		GraphicsLargeString(108, 44, "ON", WHITE);
		bit_map_screen_4.OFF = 0;
	}

	if(bit_map_screen_4.LUX == 1)
	{
		char buffer_Lux_screen4[17];
		sprintf(buffer_Lux_screen4, "%d", lux);
		GraphicsLargeString(190, 290, "LUX", BLACK);
		GraphicsLargeString(198, 275, buffer_Lux_screen4, BLACK);
		vTaskDelay(100);
		GraphicsFilledRectangle(190, 270, 50, 17, WHITE);
	}
	if(bit_map_screen_4.Temperature == 1)
	{
		char buffer_temperature_screen4[7];
		sprintf(buffer_temperature_screen4, "%.2f", Temperature);
		GraphicsLargeString(32, 290, "TEMPERATURE", BLACK);
		GraphicsLargeString(66, 275, buffer_temperature_screen4, BLACK);
		vTaskDelay(100);
		GraphicsFilledRectangle(60, 270, 70, 17, WHITE);

	}
}
