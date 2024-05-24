#ifndef MIDDLEWARE_GRAPHICS_GRAPHICS_H_
#define MIDDLEWARE_GRAPHICS_GRAPHICS_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "ili9341.h"

/*********************
 *      DEFINES
 *********************/

#define GRAPHICS_STANDARD_CHARACTER_HEIGHT 		9
#define GRAPHICS_STANDARD_CHARACTER_WIDTH 		5
#define GRAPHICS_LARGE_CHARACTER_HEIGHT 		16
#define GRAPHICS_LARGE_CHARACTER_WIDTH 			11
#define GRAPHICS_MAX_SHAPE_CORNERS				10U

/*********************
 *   GLOBAL FUNCTION
 *********************/

void GraphicsInit(void);
void GraphicsPixel(int16_t x, int16_t y, colour_t colour);
void GraphicsClear(colour_t colour);
void GraphicsVline(int16_t x, int16_t yStart, int16_t yEnd, colour_t colour);
void GraphicsHline(int16_t xStart, int16_t xEnd, int16_t y, colour_t colour);
void GraphicsRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour);
void GraphicsStandardCharacter(int16_t x, int16_t y, char c, colour_t colour);
void GraphicsLargeCharacter(int16_t x, int16_t y, char c, colour_t colour);
void GraphicsStandardCharacterVert(int16_t x, int16_t y, char c, colour_t colour);
void GraphicsLargeCharacterVert(int16_t x, int16_t y, char c, colour_t colour);
void GraphicsStandardString(int16_t x, int16_t y, const char *s, colour_t colour);
void GraphicsLargeString(int16_t x, int16_t y, const char *s, colour_t colour);
void GraphicsStandardStringVert(int16_t x, int16_t y, const char *s, colour_t colour);
void GraphicsLargeStringVert(int16_t x, int16_t y, const char *s, colour_t colour);
void GraphicsLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, colour_t colour);
void GraphicsCircle(int16_t x, int16_t y, uint16_t radius, colour_t colour);
void GraphicsColourBitmap(int16_t x, int16_t y, uint16_t width, uint16_t height, const uint8_t *imageData);
void GraphicsFilledCircle(int16_t x, int16_t y, uint16_t radius, colour_t colour);
void GraphicsFilledRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour);
void GraphicsRotateShape(uint8_t corners, int16_t *xArray, int16_t *yArray, int16_t angleDegrees);
void GraphicsScaleShape(uint8_t corners, int16_t *xArray, int16_t *yArray, int16_t scaleX, int16_t scaleY);
void GraphicsDrawShape(uint8_t corners, const int16_t *xArray, const int16_t *yArray, int16_t xOffset, int16_t yOffset, colour_t colour);
void GraphicsDrawFilledShape(uint8_t corners, const int16_t *xArray, const int16_t *yArray, int16_t x, int16_t y, colour_t colour);
void GraphicsArc(int16_t x, int16_t y, int16_t radius, int16_t startAngle, int16_t endAngle, colour_t colour);
void GraphicsSegment(int16_t x, int16_t y, uint16_t radius, int16_t startAngle, int16_t endAngle, colour_t colour);
void GraphicsFilledSegment(int16_t x, int16_t y, uint16_t radius, int16_t startAngle, int16_t endAngle, uint16_t angleStepSize, colour_t colour);
void GraphicsRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t cornerRadius, colour_t colour);

#endif /* MIDDLEWARE_GRAPHICS_GRAPHICS_H_ */
