#include "Keypad/keypad_MQTT.h"

/* position of each element in the keyboard */
static const uint16_t keypad_MQTT[13][4] = {
			{50, 85, 125, 150},	  // 1
			{103, 138, 125, 150}, // 2
			{155, 190, 125, 150}, // 3
			{50, 85, 165, 200},	  // 4
			{103, 138, 165, 200}, // 5
			{155, 190, 165, 200}, // 6
			{50, 85, 215, 240},	  // 7
			{103, 138, 215, 240}, // 8
			{155, 190, 215, 240}, // 9
			{50, 85, 260, 290},	  // X
			{103, 138, 260, 290}, // 0
			{156, 190, 260, 290}, // v
			// {105, 135, 295, 320},	  // point
			{7, 55, 8, 50},      // icon return
};

/* check touch keypad MQTT */
key_MQTT_character_t check_event_keypad_MQTT(int16_t x, int16_t y)
{
	key_MQTT_character_t i;
	for (i = NUM_1; i < NO_TOUCH; i++)
	{
		if ((x > keypad_MQTT[i][0]) && (x < keypad_MQTT[i][1]) && (y > keypad_MQTT[i][2]) && (y < keypad_MQTT[i][3]))
		{
			return i;
		}
	}
	return i;
}
