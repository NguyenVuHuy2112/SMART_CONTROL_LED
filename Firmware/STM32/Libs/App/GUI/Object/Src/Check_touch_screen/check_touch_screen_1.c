/*
 * check_touch_screen_1.c
 *
 *  Created on: Apr 15, 2024
 *      Author: vuhuy
 */

#include "Icon/icon.h"
#include "stdint.h"
#include "check_touch_screen/check_touch_screen.h"

/* position of each icon screen_1 */
static const uint16_t icon_pos[3][4] = {
    {95, 136, 51, 90},   // icon wifi
    {90, 140, 129, 177}, // icon home
    {94, 140, 208, 252}, // icon MQTT
};

/* check touch icon */
touch_icon_screen1_t check_event_icon_screen1(int16_t x, int16_t y)
{
    touch_icon_screen1_t i;
    for (i = 0; i < NO_TOUCH_ICON_SC1; i++)
    {
        if (x > icon_pos[i][0] && x < icon_pos[i][1] && y > icon_pos[i][2] && y < icon_pos[i][3])
            break;
    }
    return i;
}
