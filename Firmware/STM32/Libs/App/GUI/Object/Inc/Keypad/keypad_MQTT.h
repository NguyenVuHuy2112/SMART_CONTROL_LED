/*
 * keypad_MQTT.h
 *
 *  Created on: Apr 15, 2024
 *      Author: ACER
 */

#ifndef APP_GUI_OBJECT_INC_KEYPAD_KEYPAD_MQTT_H_
#define APP_GUI_OBJECT_INC_KEYPAD_KEYPAD_MQTT_H_

#include <stdint.h>
#include "Icon/icon.h"
#include "graphics.h"

typedef enum
{
	NUM_1 = 0,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_x,
	NUM_0,
	NUM_v,
	ICON_RETURN,
	NO_TOUCH,
} key_MQTT_character_t;

inline __attribute__((always_inline)) void object_Keypad_MQTT(void)
{
	GraphicsRoundedRectangle(50, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 125, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(50, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 170, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(50, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(155, 215, 35, 35, 5, BLACK);
	GraphicsRoundedRectangle(103, 260, 35, 35, 5, BLACK);
	//GraphicsFilledRectangle(105, 300, 30, 15, BLACK );
	//GraphicsRoundedRectangle(105, 300, 30, 15, 3, BLACK);

	GraphicsLargeCharacter(63, 134, '1', WHITE);
	GraphicsLargeCharacter(116, 134, '2', WHITE);
	GraphicsLargeCharacter(169, 134, '3', WHITE);
	GraphicsLargeCharacter(63, 179, '4', WHITE);
	GraphicsLargeCharacter(116, 179, '5', WHITE);
	GraphicsLargeCharacter(169, 179, '6', WHITE);
	GraphicsLargeCharacter(63, 225, '7', WHITE);
	GraphicsLargeCharacter(116, 225, '8', WHITE);
	GraphicsLargeCharacter(169, 225, '9', WHITE);
	GraphicsColourBitmap(52, 263, 30, 29, icon_delete);
	GraphicsLargeCharacter(116, 269, '0', WHITE);
	GraphicsColourBitmap(156, 263, 33, 33, icon_check);
	// GraphicsLargeCharacter(117, 298, '.', WHITE);
}

key_MQTT_character_t check_event_keypad_MQTT(int16_t x, int16_t y);

#endif /* APP_GUI_OBJECT_INC_KEYPAD_KEYPAD_MQTT_H_ */
