#ifndef APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_
#define APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_

/*********************
 *      INCLUDES
 *********************/

#include "stdint.h"

/**********************
 *      TYPEDEFS
 **********************/

typedef enum
{
    ICON_WIFI = 0,
    ICON_HOME,
    ICON_MQTT,
    NO_TOUCH_ICON_SC1,
} touch_icon_screen1_t;

typedef enum
{
    ICON_RETURN = 0,
    ON_OFF_WIFI,
    WIFI1,
    WIFI2,
    WIFI3,
    WIFI4,
    WIFI5,
    NEXT,
    BACK,
    NO_TOUCH_ICON_SC2,
} touch_icon_screen2_t;

typedef enum 
{
    ICON_RETURN_SC4 = 0,
    ICON_CONTROL,
    ICON_AUTO,
    NO_TOUCH_ICON_SC4,
} touch_icon_screen4_t;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

touch_icon_screen1_t check_event_icon_screen1(int16_t x, int16_t y);
touch_icon_screen2_t check_event_icon_screen2(int16_t x, int16_t y);
touch_icon_screen4_t check_event_icon_screen4(int16_t x, int16_t y);

#endif /* APP_GUI_OBJECT_INC_CHECK_TOUCH_SCREEN_CHECK_TOUCH_SCREEN_H_ */
