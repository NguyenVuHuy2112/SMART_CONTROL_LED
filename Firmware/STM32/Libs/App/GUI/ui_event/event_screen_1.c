/*********************
 *      INCLUDES
 *********************/

#include "event.h"
#include "check_touch_screen/check_touch_screen.h"
#include "screen.h"
#include "main.h"

extern int16_t x;
extern int16_t y;
extern uint8_t flag_is_touch;

extern control_led_t led_state;
extern control_auto_t auto_state;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `check_event_screen_1` checks for a touch event on a specific icon and updates the
 * event and screen state accordingly.
 *
 * @param event The `event` parameter is a pointer to a variable of type `check_event_t`, which is used
 * to store information about the current event being checked.
 * @param screen The `screen` parameter is a pointer to a `screen_state_t` structure, which likely
 * represents the current state of the screen in the application.
 */

void check_event_screen_1(screen_state_t *screen)
{
	touch_icon_screen1_t touch = check_event_icon_screen1(x, y);
	if (touch != NO_TOUCH_ICON_SC1)
	{
		if (flag_is_touch == 0)
		{
			if (touch == ICON_WIFI) /*if touch icon wifi*/
			{
				bit_map_screen_2.screen = 1;
				bit_map_screen_2.ret = 1;
				bit_map_screen_2.on_off_wifi = 1;
				bit_map_screen_2.text1 = 1;
				bit_map_screen_2.text2 = 1;
				// bit_map_screen_2.WIFI1 = 1;
				// bit_map_screen_2.WIFI2 = 1;
				// bit_map_screen_2.WIFI3 = 1;
				// bit_map_screen_2.WIFI4 = 1;
				// bit_map_screen_2.WIFI5 = 1;
				// bit_map_screen_2.NEXT = 1;
			
				*screen = SCREEN_WIFI;
			}
			else if (touch == ICON_HOME) /*if touch icon home*/
			{
				bit_map_screen_4.screen = 1;
				bit_map_screen_4.ret = 1;
				bit_map_screen_4.control = 1;
				bit_map_screen_4.automode = 1;
				bit_map_screen_4.LUX = 1; 
				bit_map_screen_4.Temperature = 1;

				if (led_state == LED_ON) bit_map_screen_4.ON = 1;
				else bit_map_screen_4.OFF = 1;

				if (auto_state == AUTO_ON) bit_map_screen_4.on_auto = 1;
				else bit_map_screen_4.off_auto = 1;
				*screen = SCREEN_MAIN;
			}
			else /*if touch icon MQTT*/
			{
				bit_map_screen_5.screen = 1;
				bit_map_screen_5.ret = 1;
				bit_map_screen_5.text = 1;
				bit_map_screen_5.frame = 1;
				bit_map_screen_5.key = 1;

				*screen = SCREEN_MQTT;
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
}
