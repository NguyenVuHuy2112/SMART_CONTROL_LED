#include "Icon/icon.h"
#include "stdint.h"
#include "check_touch_screen/check_touch_screen.h"

/* position of each icon screen_4 */
static const uint16_t icon_pos[3][4] = {
	{7, 55, 8, 50},		 // icon return
    {91, 150, 63, 123}, // icon control led
    {89, 150, 176, 236}, // icon auto control
};

/* check touch icon */
touch_icon_screen4_t check_event_icon_screen4(int16_t x, int16_t y)
{
    touch_icon_screen4_t i;
    for (i = 0; i < NO_TOUCH_ICON_SC4; i++)
    {
        if (x > icon_pos[i][0] && x < icon_pos[i][1] && y > icon_pos[i][2] && y < icon_pos[i][3])
            break;
    }
    return i;
}
