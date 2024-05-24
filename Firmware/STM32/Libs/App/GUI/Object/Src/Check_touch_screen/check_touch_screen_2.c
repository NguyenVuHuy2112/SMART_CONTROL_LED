#include "Icon/icon.h"
#include "stdint.h"
#include "check_touch_screen/check_touch_screen.h"

/* position of each icon screen_2 */
static const uint16_t frame_pos[9][4] = {
	{7, 55, 8, 50},		 // icon return
	{160, 220, 5, 55},   // On_off_wifi
	{35, 205, 110, 135}, // frame wifi 1    35, 110, 170, 25
	{35, 205, 145, 170}, // frame wifi 2    35, 145, 170, 25
	{35, 205, 180, 205}, // frame wifi 3    35, 180, 170, 25
	{35, 205, 215, 240}, // frame wifi 4    35, 215, 170, 25
	{35, 205, 250, 275}, // frame wifi 5    35, 250, 170, 25
	{160, 240, 285, 320},// frame next
	{0, 60, 285, 320},  // frame back
};

/* check touch icon */
touch_icon_screen2_t check_event_icon_screen2(int16_t x, int16_t y)
{
    touch_icon_screen2_t i;
    for (i = 0; i < NO_TOUCH_ICON_SC2; i++)
    {
        if (x > frame_pos[i][0] && x < frame_pos[i][1] && y > frame_pos[i][2] && y < frame_pos[i][3])
            break;
    }
    return i;
}
