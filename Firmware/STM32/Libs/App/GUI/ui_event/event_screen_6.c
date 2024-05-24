#include "event.h"
#include "main.h"
#include "touch.h"
#include "graphics.h"

extern uint8_t flag_is_touch;

void check_event_screen_6(screen_state_t *screen)
{
    if ( TouchIsTouched() == true)
    {
    	flag_is_touch = 1;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);    //On backlight

        bit_map_screen_1.screen = 1;        // return screen_1 if touched_screen
        bit_map_screen_1.wifi = 1;
        bit_map_screen_1.home = 1;
        bit_map_screen_1.MQTT = 1;
        *screen = SCREEN_START;
    }
}
