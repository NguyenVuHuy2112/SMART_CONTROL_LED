/*********************
 *      INCLUDES
 *********************/

#include <string.h>
#include <math.h>
#include <limits.h>
#include "graphics.h"

/*********************
 *      DEFINES
 *********************/

#define DEGREES_IN_RAD 57.2958f

/**********************
 *  STATIC VARIABLES
 **********************/

static const uint8_t Font9_Table[];
static const uint8_t Font16_Table[];

static int16_t triangleCornersX[3];
static int16_t triangleCornersY[3];

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static void arcPoint(int16_t x, int16_t y, int16_t arcX, int16_t arcY, int16_t startAngle, int16_t endAngle, colour_t colour);
static void filledRectangleClip(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour);

/*********************
 *   GLOBAL FUNCTION
 *********************/

/**
 * The function GraphicsInit initializes a graphics display using the ILI9341 driver.
 */
void GraphicsInit(void)
{
	ILI9341Reset();
	ILI9341Init();
}

/**
 * The function GraphicsClear clears the graphics display by filling a rectangle with a specified
 * color.
 *
 * @param colour The `colour` parameter in the `GraphicsClear` function is of type `colour_t`, which is
 * likely a custom data type defined elsewhere in the codebase. It represents the color that will be
 * used to clear the graphics on the display. The specific color value to be passed as an argument
 * depends
 */
void GraphicsClear(colour_t colour)
{
	filledRectangleClip(0U, 0U, (uint16_t)ILI9341_LCD_WIDTH, (uint16_t)ILI9341_LCD_HEIGHT, colour);
}

/**
 * The function GraphicsPixel sets a pixel at the specified coordinates with the specified color on a
 * display.
 *
 * @param x The parameter `x` is the x-coordinate of the pixel on the screen where you want to draw.
 * @param y The `y` parameter in the `GraphicsPixel` function represents the vertical coordinate of the
 * pixel on the screen where you want to draw. It specifies the position of the pixel along the
 * vertical axis.
 * @param colour The `colour` parameter in the `GraphicsPixel` function represents the color of the
 * pixel to be drawn on the screen. It is of type `colour_t`, which is likely a custom data type
 * defined in the codebase for representing colors. The specific values or format of the `colour_t`
 */
void GraphicsPixel(int16_t x, int16_t y, colour_t colour)
{
	ILI9341Pixel((uint16_t)x, (uint16_t)y, colour);
}

/**
 * The function GraphicsVline draws a vertical line on the screen with a specified color.
 *
 * @param x The x-coordinate of the vertical line.
 * @param yStart The starting y-coordinate of the vertical line.
 * @param yEnd The `yEnd` parameter represents the ending y-coordinate of the vertical line to be
 * drawn.
 * @param colour The `colour` parameter is the color that will be used to draw the vertical line on the
 * graphics display. It is of type `colour_t`, which likely represents a specific color format or
 * structure used in the graphics library you are working with.
 */
void GraphicsVline(int16_t x, int16_t yStart, int16_t yEnd, colour_t colour)
{
	int16_t temp;

	if (yStart > yEnd)
	{
		temp = yEnd;
		yEnd = yStart;
		yStart = temp;
	}

	filledRectangleClip(x, yStart, 1U, (uint16_t)((yEnd - yStart) + 1), colour);
}

void GraphicsHline(int16_t xStart, int16_t xEnd, int16_t y, colour_t colour)
{
	int16_t temp;

	if (xStart > xEnd)
	{
		temp = xEnd;
		xEnd = xStart;
		xStart = temp;
	}

	filledRectangleClip(xStart, y, (uint16_t)((xEnd - xStart) + 1), 1U, colour);
}

/**
 * @brief Draws a rectangle on the screen.
 *
 * This function takes the x and y coordinates, width, height, and color,
 * then draws an unfilled rectangle on the display using the provided parameters.
 * The rectangle is drawn by drawing its four sides using horizontal and vertical lines.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param colour The color used to draw the rectangle.
 *
 * @note This function relies on the GraphicsHline and GraphicsVline functions
 *       to draw the horizontal and vertical lines, respectively. Ensure these
 *       functions are properly implemented and available in the scope.
 */
void GraphicsRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour)
{
	GraphicsHline(x, x + (int16_t)width - 1, y, colour);
	GraphicsHline(x, x + (int16_t)width - 1, y + (int16_t)height - 1, colour);
	GraphicsVline(x, y, y + (int16_t)height - 1, colour);
	GraphicsVline(x + (int16_t)width - 1, y, y + (int16_t)height - 1, colour);
}

/**
 * @brief Draws a filled rectangle on the screen.
 *
 * This function takes the x and y coordinates, width, height, and color, 
 * then draws a filled rectangle on the display using the provided parameters.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param colour The color used to fill the rectangle.
 *
 * @note This function relies on the filledRectangleClip function to handle
 *       the actual drawing of the rectangle. Ensure that filledRectangleClip
 *       is properly implemented and available in the scope.
 */
void GraphicsFilledRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour)
{
	filledRectangleClip(x, y, width, height, colour);
}

/**
 * @brief Draws a standard character on the screen.
 *
 * This function takes the x and y coordinates, a character, and a color, 
 * then draws the character on the display using the provided font table.
 *
 * @param x The x-coordinate where the character drawing starts.
 * @param y The y-coordinate where the character drawing starts.
 * @param c The character to be drawn.
 * @param colour The color used to draw the character.
 *
 * @note This function relies on the Font9_Table array which defines the bitmap
 *       of each character and the ILI9341Pixel function to set individual pixels.
 *       The character height and width are defined by 
 *       GRAPHICS_STANDARD_CHARACTER_HEIGHT and GRAPHICS_STANDARD_CHARACTER_WIDTH.
 */
void GraphicsStandardCharacter(int16_t x, int16_t y, char c, colour_t colour)
{
	int8_t charX;
	int8_t charY;
	uint8_t rowByte;
	uint8_t mask;
	uint16_t byte_pos = ((int16_t)c - (int16_t)' ') * GRAPHICS_STANDARD_CHARACTER_HEIGHT;

	for (charY = 0; charY < GRAPHICS_STANDARD_CHARACTER_HEIGHT; charY++)
	{
		mask = 0x80U;
		rowByte = Font9_Table[byte_pos];
		for (charX = 0; charX < GRAPHICS_STANDARD_CHARACTER_WIDTH; charX++)
		{
			if (rowByte & mask)
			{
				ILI9341Pixel(x + (int16_t)charX, y + (int16_t)charY, colour);
			}
			mask >>= 1;
		}
		byte_pos++;
	}
}

/**
 * @brief Draws a large character on the screen.
 *
 * This function takes the x and y coordinates, a character, and a color, 
 * then draws the character on the display using the provided large font table.
 *
 * @param x The x-coordinate where the character drawing starts.
 * @param y The y-coordinate where the character drawing starts.
 * @param c The character to be drawn.
 * @param colour The color used to draw the character.
 *
 * @note This function relies on the Font16_Table array which defines the bitmap
 *       of each character and the ILI9341Pixel function to set individual pixels.
 *       The character height and width are defined by 
 *       GRAPHICS_LARGE_CHARACTER_HEIGHT and GRAPHICS_LARGE_CHARACTER_WIDTH.
 */
void GraphicsLargeCharacter(int16_t x, int16_t y, char c, colour_t colour)
{
	int8_t charX;
	int8_t charY;
	uint8_t rowByte;
	uint8_t mask;
	uint16_t byte_pos = ((int16_t)c - (int16_t)' ') * GRAPHICS_LARGE_CHARACTER_HEIGHT * 2U;

	for (charY = 0; charY < GRAPHICS_LARGE_CHARACTER_HEIGHT; charY++)
	{
		mask = 0x80U;
		rowByte = Font16_Table[byte_pos];
		for (charX = 0; charX < GRAPHICS_LARGE_CHARACTER_WIDTH; charX++)
		{
			if (rowByte & mask)
			{
				ILI9341Pixel(x + (int16_t)charX, y + (int16_t)charY, colour);
			}
			mask >>= 1;
			if (mask == 0U)
			{
				mask = 0x80U;
				byte_pos++;
				rowByte = Font16_Table[byte_pos];
			}
		}
		byte_pos++;
	}
}

/**
 * @brief Draws a standard character vertically on the screen.
 *
 * This function takes the x and y coordinates, a character, and a color, 
 * then draws the character vertically on the display using the provided font table.
 *
 * @param x The x-coordinate where the character drawing starts.
 * @param y The y-coordinate where the character drawing starts.
 * @param c The character to be drawn.
 * @param colour The color used to draw the character.
 *
 * @note This function relies on the Font9_Table array which defines the bitmap
 *       of each character and the ILI9341Pixel function to set individual pixels.
 *       The character height and width are defined by 
 *       GRAPHICS_STANDARD_CHARACTER_HEIGHT and GRAPHICS_STANDARD_CHARACTER_WIDTH.
 */
void GraphicsStandardCharacterVert(int16_t x, int16_t y, char c, colour_t colour)
{
	int8_t charX;
	int8_t charY;
	uint8_t rowByte;
	uint8_t mask;
	uint16_t byte_pos = ((int16_t)c - (int16_t)' ') * GRAPHICS_STANDARD_CHARACTER_HEIGHT;

	for (charY = 0; charY < GRAPHICS_STANDARD_CHARACTER_HEIGHT; charY++)
	{
		mask = 0x80U;
		rowByte = Font9_Table[byte_pos];
		for (charX = 0; charX < GRAPHICS_STANDARD_CHARACTER_WIDTH; charX++)
		{
			if (rowByte & mask)
			{
				ILI9341Pixel(x + GRAPHICS_STANDARD_CHARACTER_HEIGHT - (int16_t)charY - 1U, y + (int16_t)charX, colour);
			}
			mask >>= 1;
		}
		byte_pos++;
	}
}

void GraphicsLargeCharacterVert(int16_t x, int16_t y, char c, colour_t colour)
{
	int8_t charX;
	int8_t charY;
	uint8_t rowByte;
	uint8_t mask;
	uint16_t byte_pos = ((int16_t)c - (int16_t)' ') * GRAPHICS_LARGE_CHARACTER_HEIGHT * 2U;

	for (charY = 0; charY < GRAPHICS_LARGE_CHARACTER_HEIGHT; charY++)
	{
		mask = 0x80U;
		rowByte = Font16_Table[byte_pos];
		for (charX = 0; charX < GRAPHICS_LARGE_CHARACTER_WIDTH; charX++)
		{
			if (rowByte & mask)
			{
				ILI9341Pixel(x + GRAPHICS_LARGE_CHARACTER_HEIGHT - (int16_t)charY - 1U, y + (int16_t)charX, colour);
			}
			mask >>= 1;
			if (mask == 0U)
			{
				mask = 0x80U;
				byte_pos++;
				rowByte = Font16_Table[byte_pos];
			}
		}
		byte_pos++;
	}
}

/**
 * The function GraphicsLine draws a line on a graphics display using the Bresenham line drawing
 * algorithm.
 * 
 * @param x1 `x1` is the x-coordinate of the starting point of the line.
 * @param y1 The parameter `y1` represents the y-coordinate of the starting point of the line.
 * @param x2 x2 is the x-coordinate of the end point of the line.
 * @param y2 The parameter `y2` represents the y-coordinate of the end point of the line segment you
 * want to draw. It is the vertical position where the line should end on the screen.
 * @param colour The `colour` parameter in the `GraphicsLine` function is of type `colour_t`, which is
 * likely a custom data type specific to the graphics library or framework you are using. This data
 * type represents the color that will be used to draw the line on the screen. The exact definition and
 * range
 */
void GraphicsLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, colour_t colour)
{
	int16_t F;
	int16_t x;
	int16_t y;
	int16_t dy;
	int16_t dx;
	int16_t dy2;
	int16_t dx2;
	int16_t dy2_minus_dx2;
	int16_t dy2_plus_dx2;

	if (x1 > x2)
	{
		F = x1;
		x1 = x2;
		x2 = F;

		F = y1;
		y1 = y2;
		y2 = F;
	}

	dy = y2 - y1;
	dx = x2 - x1;
	dy2 = (dy << 1);
	dx2 = (dx << 1);
	dy2_minus_dx2 = dy2 - dx2;
	dy2_plus_dx2 = dy2 + dx2;

	if (dy >= 0)
	{
		if (dy <= dx)
		{
			F = dy2 - dx;
			x = x1;
			y = y1;
			while (x <= x2)
			{
				ILI9341Pixel(x, y, colour);

				if (F <= 0)
				{
					F += dy2;
				}
				else
				{
					y++;
					F += dy2_minus_dx2;
				}
				x++;
			}
		}
		else
		{
			F = dx2 - dy;
			y = y1;
			x = x1;
			while (y <= y2)
			{
				ILI9341Pixel(x, y, colour);
				if (F <= 0)
				{
					F += dx2;
				}
				else
				{
					x++;
					F -= dy2_minus_dx2;
				}
				y++;
			}
		}
	}
	else
	{
		if (dx >= -dy)
		{
			F = -dy2 - dx;
			x = x1;
			y = y1;
			while (x <= x2)
			{
				ILI9341Pixel(x, y, colour);
				if (F <= 0)
				{
					F -= dy2;
				}
				else
				{
					y--;
					F -= dy2_plus_dx2;
				}
				x++;
			}
		}
		else
		{
			F = dx2 + dy;
			y = y1;
			x = x1;
			while (y >= y2)
			{
				ILI9341Pixel(x, y, colour);
				if (F <= 0)
				{
					F += dx2;
				}
				else
				{
					x++;
					F += dy2_plus_dx2;
				}
				y--;
			}
		}
	}
}

/**
 * The function GraphicsCircle draws a circle on a graphics display using the midpoint circle
 * algorithm.
 * 
 * @param x The parameter `x` represents the x-coordinate of the center of the circle on the screen.
 * @param y The `y` parameter in the `GraphicsCircle` function represents the y-coordinate of the
 * center of the circle to be drawn on the screen.
 * @param radius The `radius` parameter in the `GraphicsCircle` function represents the radius of the
 * circle to be drawn. It determines the size of the circle that will be plotted on the screen.
 * @param colour The `colour` parameter in the `GraphicsCircle` function represents the color that will
 * be used to draw the circle on the screen. It is of type `colour_t`, which is likely a custom data
 * type defined in the codebase for handling colors in the graphics library. The specific
 * implementation of `
 */
void GraphicsCircle(int16_t x, int16_t y, uint16_t radius, colour_t colour)
{
	int16_t xPoint = 0;
	int16_t yPoint = (int16_t)radius;
	int16_t decision = 1 - radius;

	// plot top, bottom, left and right
	ILI9341Pixel(x, y + radius, colour);
	ILI9341Pixel(x, y - radius, colour);
	ILI9341Pixel(x + radius, y, colour);
	ILI9341Pixel(x - radius, y, colour);

	while (yPoint > xPoint)
	{
		if (decision < 0)
		{
			decision += (xPoint << 1) + 3;
			xPoint++;
		}
		else
		{
			decision += ((xPoint - yPoint) << 1) + 5;
			xPoint++;
			yPoint--;
		}

		// plot all quadrants
		ILI9341Pixel(x + xPoint, y + yPoint, colour);
		ILI9341Pixel(x - xPoint, y + yPoint, colour);
		ILI9341Pixel(x + xPoint, y - yPoint, colour);
		ILI9341Pixel(x - xPoint, y - yPoint, colour);
		ILI9341Pixel(x + yPoint, y + xPoint, colour);
		ILI9341Pixel(x - yPoint, y + xPoint, colour);
		ILI9341Pixel(x + yPoint, y - xPoint, colour);
		ILI9341Pixel(x - yPoint, y - xPoint, colour);
	}
}

/**
 * The function GraphicsColourBitmap displays a colour bitmap image on a graphics display at the
 * specified coordinates.
 * 
 * @param x The `x` parameter represents the x-coordinate of the top-left corner where the bitmap will
 * be drawn on the screen.
 * @param y The `y` parameter represents the vertical position where the top-left corner of the bitmap
 * will be drawn on the screen.
 * @param width The `width` parameter specifies the width of the bitmap image in pixels.
 * @param height The `height` parameter in the `GraphicsColourBitmap` function represents the vertical
 * dimension of the bitmap image that will be drawn on the screen. It specifies the number of rows or
 * pixels in the image from top to bottom.
 * @param imageData The `imageData` parameter is a pointer to an array of `uint8_t` data that
 * represents the pixel values of an image. Each element in the array corresponds to a pixel in the
 * image, and the values typically represent the color or grayscale intensity of that pixel.
 */
void GraphicsColourBitmap(int16_t x, int16_t y, uint16_t width, uint16_t height, const uint8_t *imageData)
{
	ILI9341DrawColourBitmap(x, y, width, height, imageData);
}

/**
 * The function GraphicsStandardString displays a string character by character at specified
 * coordinates with a specified color.
 * 
 * @param x The x-coordinate where the string will be displayed on the screen.
 * @param y The `y` parameter in the `GraphicsStandardString` function represents the vertical position
 * where the string will be displayed on the screen.
 * @param s The parameter `s` is a pointer to a constant character array (string) that contains the
 * text to be displayed on the graphics screen.
 * @param colour The `colour` parameter represents the color in which the string will be displayed on
 * the graphics screen.
 */
void GraphicsStandardString(int16_t x, int16_t y, const char *s, colour_t colour)
{
	size_t c;
	size_t length;

	length = strlen(s);
	for (c = (size_t)0; c < length; c++)
	{
		GraphicsStandardCharacter(x + (int16_t)c * (GRAPHICS_STANDARD_CHARACTER_WIDTH + 1), y, s[c], colour);
	}
}

/**
 * The function GraphicsLargeString displays a large string at a specified position on the screen with
 * a specified color.
 * 
 * @param x The x parameter is the horizontal position where the large string will be displayed on the
 * screen.
 * @param y The `y` parameter in the `GraphicsLargeString` function represents the vertical position
 * where the large string will be displayed on the screen. It determines the height or the row where
 * the string will start being drawn.
 * @param s The parameter `s` is a pointer to a constant character array (string) that contains the
 * text to be displayed on the graphics screen.
 * @param colour The `colour` parameter in the `GraphicsLargeString` function is of type `colour_t`,
 * which is likely a custom data type defined elsewhere in the codebase. This data type is used to
 * represent colors in the graphics system. The specific definition and usage of the `colour_t` type
 * would
 */
void GraphicsLargeString(int16_t x, int16_t y, const char *s, colour_t colour)
{
	size_t c;
	size_t length;

	length = strlen(s);
	for (c = (size_t)0; c < length; c++)
	{
		GraphicsLargeCharacter(x + (int16_t)c * (GRAPHICS_LARGE_CHARACTER_WIDTH + 1), y, s[c], colour);
	}
}

/**
 * The function GraphicsStandardStringVert displays a string vertically by iterating through each
 * character and calling another function to display it.
 * 
 * @param x The `x` parameter is the horizontal position where the string will be drawn on the screen.
 * @param y The `y` parameter in the `GraphicsStandardStringVert` function represents the vertical
 * position where the string will be displayed on the screen.
 * @param s The parameter `s` is a pointer to a constant character array (string) that contains the
 * text to be displayed vertically.
 * @param colour The `colour` parameter in the `GraphicsStandardStringVert` function is of type
 * `colour_t`, which is likely a custom data type defined elsewhere in the codebase. This data type is
 * used to represent colors in the graphics system. The specific definition and usage of `colour_t`
 * would be
 */
void GraphicsStandardStringVert(int16_t x, int16_t y, const char *s, colour_t colour)
{
	size_t c;
	size_t length;

	length = strlen(s);
	for (c = (size_t)0; c < length; c++)
	{
		GraphicsStandardCharacterVert(x, y + (int16_t)c * (GRAPHICS_STANDARD_CHARACTER_WIDTH + 1), s[c], colour);
	}
}

/**
 * The function GraphicsLargeStringVert displays a large string vertically by iterating through each
 * character and calling another function to display it.
 * 
 * @param x The x parameter is the horizontal position where the large string will be drawn on the
 * screen.
 * @param y The `y` parameter in the `GraphicsLargeStringVert` function represents the vertical
 * position where the large string will be displayed on the screen.
 * @param s The parameter `s` is a pointer to a constant character array (string) that contains the
 * text to be displayed vertically.
 * @param colour The `colour` parameter is the color that will be used to display the string on the
 * graphics screen. It is of type `colour_t`, which likely represents a specific color format or data
 * type used in the graphics library you are working with. You would typically pass a color value or
 * reference to specify
 */
void GraphicsLargeStringVert(int16_t x, int16_t y, const char *s, colour_t colour)
{
	size_t c;
	size_t length;

	length = strlen(s);
	for (c = (size_t)0; c < length; c++)
	{
		GraphicsLargeCharacterVert(x, y + (int16_t)c * (GRAPHICS_LARGE_CHARACTER_WIDTH + 1), s[c], colour);
	}
}

/**
 * The function GraphicsFilledCircle draws a filled circle with a specified radius and color at a given
 * position on the screen using rectangles.
 * 
 * @param x The parameter `x` represents the x-coordinate of the center of the filled circle on the
 * screen.
 * @param y The `y` parameter in the `GraphicsFilledCircle` function represents the y-coordinate of the
 * center of the filled circle to be drawn on the screen.
 * @param radius The `radius` parameter in the `GraphicsFilledCircle` function represents the radius of
 * the filled circle that will be drawn on the screen. It determines the size of the circle.
 * @param colour The `colour` parameter in the `GraphicsFilledCircle` function represents the color
 * that will be used to fill the circle on the graphics display. It is of type `colour_t`, which is a
 * custom data type specific to the graphics library or framework you are using. The actual color value
 * will be
 */
void GraphicsFilledCircle(int16_t x, int16_t y, uint16_t radius, colour_t colour)
{
	int16_t xPoint = 0U;
	int16_t yPoint = (int16_t)radius;
	int16_t decision = 1 - (int16_t)radius;

	// plot centre line
	filledRectangleClip(x, y - (int16_t)radius, 1U, radius * 2U + 1U, colour);

	while (yPoint > xPoint)
	{
		if (decision < 0)
		{
			decision += (xPoint << 1) + 3;
			xPoint++;
		}
		else
		{
			decision += ((xPoint - yPoint) << 1) + 5;
			xPoint++;
			yPoint--;
		}

		// plot all quadrants
		filledRectangleClip(x + xPoint, y - yPoint, 1U, (uint16_t)yPoint * 2U + 1U, colour);
		filledRectangleClip(x - xPoint, y - yPoint, 1U, (uint16_t)yPoint * 2U + 1U, colour);
		filledRectangleClip(x + yPoint, y - xPoint, 1U, (uint16_t)xPoint * 2U + 1U, colour);
		filledRectangleClip(x - yPoint, y - xPoint, 1U, (uint16_t)xPoint * 2U + 1U, colour);
	}
}

/**
 * The function GraphicsRotateShape rotates a shape defined by arrays of x and y coordinates by a
 * specified angle in degrees.
 * 
 * @param corners The `corners` parameter represents the number of corners in the shape that you want
 * to rotate. It indicates the number of points in the arrays `xArray` and `yArray` that form the
 * shape.
 * @param xArray The `xArray` parameter is a pointer to an array of `int16_t` values representing the
 * x-coordinates of the corners of a shape.
 * @param yArray The `yArray` parameter in the `GraphicsRotateShape` function is a pointer to an array
 * of `int16_t` values representing the y-coordinates of the corners of a shape that you want to
 * rotate.
 * @param angleDegrees The `angleDegrees` parameter in the `GraphicsRotateShape` function represents
 * the angle by which the shape will be rotated in degrees. This angle determines the amount of
 * rotation applied to the shape around its center point.
 */
void GraphicsRotateShape(uint8_t corners, int16_t *xArray, int16_t *yArray, int16_t angleDegrees)
{
	uint8_t i;
	int16_t x;
	float angleRadians;

	angleRadians = (float)angleDegrees / DEGREES_IN_RAD;
	for (i = 0U; i < corners; i++)
	{
		x = (int16_t)((float)xArray[i] * cosf(angleRadians) - (float)yArray[i] * sinf(angleRadians));
		yArray[i] = (int16_t)((float)xArray[i] * sinf(angleRadians) + (float)yArray[i] * cosf(angleRadians));
		xArray[i] = x;
	}
}

/**
 * The function `GraphicsScaleShape` scales the coordinates of a shape by the specified factors
 * `scaleX` and `scaleY`.
 * 
 * @param corners The `corners` parameter represents the number of corners in the shape that you want
 * to scale. It is used to determine the number of elements in the `xArray` and `yArray` arrays that
 * store the coordinates of the shape's corners.
 * @param xArray The `xArray` parameter is a pointer to an array of `int16_t` values representing the
 * x-coordinates of the corners of a shape.
 * @param yArray The `yArray` parameter is a pointer to an array of `int16_t` data type, which contains
 * the y-coordinates of the shape vertices.
 * @param scaleX The `scaleX` parameter represents the scaling factor for the x-coordinates of the
 * shape. It is used to scale the x-coordinates of the shape by multiplying each x-coordinate by
 * `scaleX / 10`.
 * @param scaleY The `scaleY` parameter in the `GraphicsScaleShape` function represents the scaling
 * factor for the y-coordinates of the shape being drawn. It is used to scale the y-coordinates of the
 * shape by multiplying each y-coordinate in the `yArray` by `scaleY / 10`.
 */
void GraphicsScaleShape(uint8_t corners, int16_t *xArray, int16_t *yArray, int16_t scaleX, int16_t scaleY)
{
	uint8_t i;

	for (i = 0U; i < corners; i++)
	{
		xArray[i] = xArray[i] * scaleX / 10;
		yArray[i] = yArray[i] * scaleY / 10;
	}
}

/**
 * The function GraphicsDrawShape draws a shape by connecting the specified corners with lines of a
 * specified color.
 * 
 * @param corners The `corners` parameter represents the number of corners or vertices of the shape to
 * be drawn.
 * @param xArray The `xArray` parameter is a pointer to an array of `int16_t` values representing the
 * x-coordinates of the corners of the shape to be drawn. Each element in the array corresponds to the
 * x-coordinate of a corner point of the shape.
 * @param yArray The `yArray` parameter is a pointer to an array of `int16_t` values representing the
 * y-coordinates of the corners of the shape to be drawn. Each element in the array corresponds to the
 * y-coordinate of a corner of the shape.
 * @param xOffset The `xOffset` parameter in the `GraphicsDrawShape` function is used to specify the
 * horizontal offset by which the shape should be shifted from its original position before drawing it
 * on the screen.
 * @param yOffset The `yOffset` parameter in the `GraphicsDrawShape` function represents the vertical
 * offset by which the shape will be shifted from its original position along the y-axis when drawn on
 * the screen. It allows you to position the shape at a specific vertical location relative to the
 * origin.
 * @param colour The `colour` parameter in the `GraphicsDrawShape` function is of type `colour_t`,
 * which is used to specify the color of the shape being drawn. The actual definition of `colour_t` and
 * the available color options would be defined elsewhere in the codebase or in a library that you
 */
void GraphicsDrawShape(uint8_t corners, const int16_t *xArray, const int16_t *yArray, int16_t xOffset, int16_t yOffset, colour_t colour)
{
	uint8_t i;

	for (i = 0U; i < corners - 1U; i++)
	{
		GraphicsLine(xArray[i] + xOffset, yArray[i] + yOffset, xArray[i + 1U] + xOffset, yArray[i + 1U] + yOffset, colour);
	}

	GraphicsLine(xArray[i] + xOffset, yArray[i] + yOffset, xArray[0] + xOffset, yArray[0] + yOffset, colour);
}

/**
 * The function `GraphicsDrawFilledShape` fills a shape defined by its corners with a specified color
 * on a graphics display.
 * 
 * @param corners The `corners` parameter represents the number of corners in the shape you want to
 * draw. It indicates how many points define the shape you are drawing.
 * @param xArray The `xArray` parameter is a pointer to an array of `int16_t` values representing the
 * x-coordinates of the corners of the shape to be drawn. Each element in the array corresponds to the
 * x-coordinate of a corner of the shape.
 * @param yArray The `yArray` parameter is an array of `int16_t` values that represent the
 * y-coordinates of the corners of the shape to be drawn. The number of elements in this array is
 * determined by the `corners` parameter, which specifies the total number of corners in the shape.
 * @param x The `x` parameter in the `GraphicsDrawFilledShape` function represents the x-coordinate of
 * the shape's position on the screen. It is used to determine the horizontal position where the shape
 * will be drawn.
 * @param y The parameter `y` in the `GraphicsDrawFilledShape` function represents the vertical
 * position where the shape will be drawn on the screen. It is the y-coordinate of the top-left corner
 * of the shape relative to the screen or drawing area.
 * @param colour The `colour` parameter in the `GraphicsDrawFilledShape` function is of type
 * `colour_t`, which is used to specify the color of the filled shape that will be drawn on the screen.
 * The specific definition of the `colour_t` type is not provided in the code snippet you shared,
 * 
 * @return If the condition `(yNext + y > (int16_t)ILI9341_LCD_HEIGHT)` is true, the function will
 * return without executing the rest of the code.
 */
void GraphicsDrawFilledShape(uint8_t corners, const int16_t *xArray, const int16_t *yArray, int16_t x, int16_t y, colour_t colour)
{
	uint16_t i;
	uint16_t j;
	int16_t swap;
	uint16_t line_node_count;
	int16_t yNext;
	int16_t node_x[GRAPHICS_MAX_SHAPE_CORNERS - 1U];
	int16_t yMin = INT16_MAX;
	int16_t yMax = INT16_MIN;
	float temp;

	// find y range of shape
	for (i = 0U; i < corners; i++)
	{
		if (yArray[i] < yMin)
		{
			yMin = yArray[i];
		}
		if (yArray[i] > yMax)
		{
			yMax = yArray[i];
		}
	}

	//  Loop through the rows of the image
	for (yNext = yMin; yNext <= yMax; yNext++)
	{
		if (yNext + y > (int16_t)ILI9341_LCD_HEIGHT)
		{
			return;
		}

		// build a list of nodes on this line
		line_node_count = 0U;
		j = corners - 1U;
		for (i = 0U; i < corners; i++)
		{
			if ((yArray[i] < yNext && yArray[j] >= yNext) || (yArray[j] < yNext && yArray[i] >= yNext))
			{
				temp = (float)(yNext - yArray[i]) / (float)((yArray[j] - yArray[i]));
				node_x[line_node_count++] = (int16_t)(xArray[i] + temp * ((xArray[j] - xArray[i])));
			}
			j = i;
		}

		// sort the nodes, via a simple bubble sort
		i = 0U;
		while (i + 1U < line_node_count)
		{
			if (node_x[i] > node_x[i + 1U])
			{
				swap = node_x[i];
				node_x[i] = node_x[i + 1U];
				node_x[i + 1U] = swap;
				if (i)
				{
					i--;
				}
			}
			else
			{
				i++;
			}
		}

		// fill the pixels between node pairs
		for (i = 0U; i < line_node_count; i += 2U)
		{
			node_x[i] += x;
			node_x[i + 1U] += x;

			if (node_x[i] > (int16_t)ILI9341_LCD_WIDTH)
			{
				break;
			}
			if (node_x[i + 1U] > 0)
			{
				filledRectangleClip(node_x[i], yNext + y, (uint16_t)(node_x[i + 1U] - node_x[i] + 1), 1U, colour);
			}
		}
	}
}

/**
 * The function GraphicsArc draws an arc with a specified radius, start angle, end angle, and color.
 * 
 * @param x The parameter `x` represents the x-coordinate of the center of the arc on the screen.
 * @param y The `y` parameter in the `GraphicsArc` function represents the y-coordinate of the center
 * point of the arc. It is used to determine the position of the arc within the graphics space.
 * @param radius The `radius` parameter in the `GraphicsArc` function represents the radius of the arc
 * that will be drawn. It defines the distance from the center point `(x, y)` to the outer edge of the
 * arc.
 * @param startAngle The `startAngle` parameter represents the starting angle of the arc in degrees. It
 * determines where the arc begins drawing from.
 * @param endAngle The `endAngle` parameter in the `GraphicsArc` function represents the ending angle
 * of the arc to be drawn. It specifies the angle at which the arc should end, measured in degrees. The
 * arc will be drawn from the `startAngle` to this `endAngle` in a clockwise direction
 * @param colour The `colour` parameter in the `GraphicsArc` function is of type `colour_t`, which is
 * likely a custom data type defined elsewhere in your codebase to represent colors. You can use this
 * parameter to specify the color in which the arc will be drawn on the graphics display. The exact
 * definition
 */
void GraphicsArc(int16_t x, int16_t y, int16_t radius, int16_t startAngle, int16_t endAngle, colour_t colour)
{
	int16_t xPoint = 0;
	int16_t yPoint = radius;
	int16_t decision = 1 - radius;

	if (startAngle < 0)
	{
		startAngle += 360;
	}

	if (endAngle < 0)
	{
		endAngle += 360;
	}

	// plot top, bottom, left and right
	arcPoint(x, y, x, y + radius, startAngle, endAngle, colour); // bottom
	arcPoint(x, y, x, y - radius, startAngle, endAngle, colour); // top
	arcPoint(x, y, x + radius, y, startAngle, endAngle, colour); // right
	arcPoint(x, y, x - radius, y, startAngle, endAngle, colour); // left

	while (yPoint > xPoint)
	{
		if (decision < 0)
		{
			decision += (xPoint << 1) + 3;
			xPoint++;
		}
		else
		{
			decision += ((xPoint - yPoint) << 1) + 5;
			xPoint++;
			yPoint--;
		}

		// plot all quadrants
		arcPoint(x, y, x + xPoint, y + yPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x - xPoint, y + yPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x + xPoint, y - yPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x - xPoint, y - yPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x + yPoint, y + xPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x - yPoint, y + xPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x + yPoint, y - xPoint, startAngle, endAngle, colour);
		arcPoint(x, y, x - yPoint, y - xPoint, startAngle, endAngle, colour);
	}
}

/**
 * The function GraphicsSegment draws a segment of a circle defined by a radius, start angle, and end
 * angle with a specified color.
 * 
 * @param x The parameter `x` represents the x-coordinate of the center point of the segment on the
 * graphics display.
 * @param y The `y` parameter in the `GraphicsSegment` function represents the y-coordinate of the
 * center point of the segment on the graphics display. It is used to determine the position of the
 * segment relative to the y-axis.
 * @param radius The `radius` parameter in the `GraphicsSegment` function represents the length of the
 * line segment from the center point `(x, y)` to the outer edge of the segment. It determines how long
 * the segment will be from the center.
 * @param startAngle The `startAngle` parameter in the `GraphicsSegment` function represents the
 * starting angle of the segment to be drawn. It is an angle in degrees that specifies where the
 * segment should begin.
 * @param endAngle The `endAngle` parameter in the `GraphicsSegment` function represents the angle at
 * which the segment ends. It is used to determine the end point of the segment within the specified
 * radius from the center point (x, y) based on the angle provided.
 * @param colour The `colour` parameter in the `GraphicsSegment` function represents the color that
 * will be used to draw the segment on the graphics display. It is of type `colour_t`, which is likely
 * a custom data type defined in the graphics library you are using. This parameter specifies the color
 * of the segment
 */
void GraphicsSegment(int16_t x, int16_t y, uint16_t radius, int16_t startAngle, int16_t endAngle, colour_t colour)
{
	GraphicsLine(x,
				 y,
				 x + (int16_t)(sinf((float)startAngle / DEGREES_IN_RAD) * (float)radius),
				 y - (int16_t)(cosf((float)startAngle / DEGREES_IN_RAD) * (float)radius),
				 colour);

	GraphicsLine(x,
				 y,
				 x + (int16_t)(sinf((float)endAngle / DEGREES_IN_RAD) * (float)radius),
				 y - (int16_t)(cosf((float)endAngle / DEGREES_IN_RAD) * (float)radius),
				 colour);

	GraphicsArc(x, y, radius, startAngle, endAngle, colour);
}

/**
 * The function GraphicsFilledSegment draws a filled segment of a circle with specified parameters.
 * 
 * @param x The parameter `x` represents the x-coordinate of the center point of the segment on the
 * graphics display.
 * @param y The `y` parameter in the `GraphicsFilledSegment` function represents the y-coordinate of
 * the center point of the filled segment on the graphics display. It is used to position the segment
 * vertically on the screen.
 * @param radius The `radius` parameter represents the radius of the segment, which is the distance
 * from the center point `(x, y)` to the outer edge of the segment.
 * @param startAngle The `startAngle` parameter represents the starting angle of the filled segment in
 * degrees. It determines where the filled segment will begin drawing from in a circular manner.
 * @param endAngle The `endAngle` parameter represents the ending angle of the segment to be filled in
 * degrees. This function draws a filled segment of a circle starting from `startAngle` and ending at
 * `endAngle`. If the `startAngle` is greater than the `endAngle`, it will be adjusted to
 * @param angleStepSize The `angleStepSize` parameter in the `GraphicsFilledSegment` function
 * represents the increment value for the angle as the function draws the filled segment. It determines
 * how finely the segment will be divided into smaller sections for drawing. A smaller `angleStepSize`
 * will result in a smoother curve but may
 * @param colour The `colour` parameter in the `GraphicsFilledSegment` function is of type `colour_t`,
 * which is likely a custom data type defined elsewhere in your codebase to represent colors. It is
 * used to specify the color with which the filled segment will be drawn on the graphics display. The
 * exact definition
 */
void GraphicsFilledSegment(int16_t x, int16_t y, uint16_t radius, int16_t startAngle, int16_t endAngle, uint16_t angleStepSize, colour_t colour)
{
	int16_t oldX = x + (int16_t)(sinf((float)startAngle / DEGREES_IN_RAD) * (float)radius);
	int16_t oldY = y - (int16_t)(cosf((float)startAngle / DEGREES_IN_RAD) * (float)radius);
	int16_t i;
	float angle;

	triangleCornersX[0] = x;
	triangleCornersY[0] = y;

	if (startAngle > endAngle)
	{
		endAngle += 360;
	}

	for (i = startAngle + (int16_t)angleStepSize; i <= endAngle; i += (int16_t)angleStepSize)
	{
		angle = (float)i / DEGREES_IN_RAD;
		triangleCornersX[1] = x + (int16_t)(sinf(angle) * (float)radius);
		triangleCornersY[1] = y - (int16_t)(cosf(angle) * (float)radius);
		triangleCornersX[2] = oldX;
		triangleCornersY[2] = oldY;

		GraphicsDrawFilledShape(3U, triangleCornersX, triangleCornersY, 0, 0, colour);

		oldX = triangleCornersX[1];
		oldY = triangleCornersY[1];
	}

	i -= angleStepSize;
	if (i < endAngle)
	{
		triangleCornersX[1] = x + (int16_t)(sinf(endAngle / DEGREES_IN_RAD) * (float)radius);
		triangleCornersY[1] = y - (int16_t)(cosf(endAngle / DEGREES_IN_RAD) * (float)radius);
		triangleCornersX[2] = x + (int16_t)(sinf(i / DEGREES_IN_RAD) * (float)radius);
		triangleCornersY[2] = y - (int16_t)(cosf(i / DEGREES_IN_RAD) * (float)radius);

		GraphicsDrawFilledShape(3U, triangleCornersX, triangleCornersY, 0, 0, colour);
	}
}

/**
 * The function GraphicsRoundedRectangle draws a rounded rectangle with specified dimensions and corner
 * radius using filled rectangles and segments.
 * 
 * @param x The `x` parameter represents the x-coordinate of the top-left corner of the rounded
 * rectangle on the screen.
 * @param y The `y` parameter in the `GraphicsRoundedRectangle` function represents the y-coordinate of
 * the top-left corner of the rounded rectangle on the screen or canvas where you are drawing. It
 * specifies the vertical position where the rectangle will be drawn.
 * @param width The `width` parameter specifies the width of the rounded rectangle in pixels. It
 * determines how wide the rectangle will be on the screen.
 * @param height The `height` parameter in the `GraphicsRoundedRectangle` function represents the
 * vertical size of the rounded rectangle that will be drawn on the screen. It specifies the distance
 * from the top edge to the bottom edge of the rectangle.
 * @param cornerRadius The `cornerRadius` parameter in the `GraphicsRoundedRectangle` function
 * represents the radius of the rounded corners of the rectangle that will be drawn on the screen. It
 * determines how rounded the corners of the rectangle will be. The larger the `cornerRadius`, the more
 * rounded the corners will appear.
 * @param colour The `colour` parameter in the `GraphicsRoundedRectangle` function is used to specify
 * the color of the rounded rectangle that will be drawn on the screen. You can provide a color value
 * or reference to indicate the desired color for the rectangle.
 * 
 * @return If the width or height is less than twice the corner radius, the function will return
 * without drawing anything.
 */
void GraphicsRoundedRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t cornerRadius, colour_t colour)
{
	if (width < cornerRadius * 2 || height < cornerRadius * 2)
	{
		return;
	}

	GraphicsFilledRectangle(x + (int16_t)cornerRadius, y, width - (2 * cornerRadius), height, colour);
	GraphicsFilledRectangle(x, y + (int16_t)cornerRadius, cornerRadius + 1, height - (2 * cornerRadius) + 4, colour);
	GraphicsFilledRectangle(x + width - (int16_t)cornerRadius, y + (int16_t)cornerRadius, cornerRadius, height - (2 * cornerRadius) + 4, colour);

	GraphicsFilledSegment(x + (int16_t)cornerRadius, y + (int16_t)cornerRadius, cornerRadius, 270, 360, 15U, colour);
	GraphicsFilledSegment(x + width - (int16_t)cornerRadius - 1, y + (int16_t)cornerRadius, cornerRadius, 0, 90, 15U, colour);
	GraphicsFilledSegment(x + (int16_t)cornerRadius, y + height - (int16_t)cornerRadius - 1, cornerRadius, 180, 270, 15U, colour);
	GraphicsFilledSegment(x + width - (int16_t)cornerRadius - 1, y + height - (int16_t)cornerRadius - 1, cornerRadius, 90, 180, 15U, colour);
}

/********************
 *  STATIC FUNCTION
 ********************/

/**
 * The function arcPoint calculates the angle of a point relative to a circle center and determines if
 * it falls within a specified range of angles before coloring the pixel at that point.
 * 
 * @param x The `x` parameter represents the x-coordinate of the point you want to check in relation to
 * the arc.
 * @param y The `y` parameter in the `arcPoint` function represents the y-coordinate of the current
 * point on the screen where you want to draw an arc.
 * @param arcX The `arcX` parameter represents the x-coordinate of the center of the arc.
 * @param arcY The `arcY` parameter represents the y-coordinate of the center of the circle.
 * @param startAngle The `startAngle` parameter represents the starting angle of the arc in degrees.
 * @param endAngle The `endAngle` parameter represents the ending angle of the arc in degrees.
 * @param colour The `colour` parameter in the `arcPoint` function represents the color that will be
 * used to draw a pixel at the specified coordinates `(arcX, arcY)` if the conditions in the function
 * are met. The color will be applied to the pixel drawn on the display.
 */
static void arcPoint(int16_t x, int16_t y, int16_t arcX, int16_t arcY, int16_t startAngle, int16_t endAngle, colour_t colour)
{
	// calculate the angle the current point makes with the circle centre
	int16_t angle = (int16_t)(DEGREES_IN_RAD * (atan2f((float)(y - arcY), (float)(x - arcX)))) - 90.0f;
	if (angle < 0)
	{
		angle += 360;
	}

	if (endAngle > startAngle)
	{
		if (angle >= startAngle && angle < endAngle)
		{
			ILI9341Pixel(arcX, arcY, colour);
		}
	}
	else
	{
		if (!(angle >= endAngle && angle < startAngle))
		{
			ILI9341Pixel(arcX, arcY, colour);
		}
	}
}

/**
 * The function `filledRectangleClip` draws a filled rectangle on a display with clipping to ensure it
 * stays within the bounds of the screen.
 * 
 * @param x The parameter `x` represents the x-coordinate of the top-left corner of the rectangle to be
 * drawn on the screen.
 * @param y The parameter `y` represents the vertical position of the top-left corner of the filled
 * rectangle on the display.
 * @param width The `width` parameter represents the width of the rectangle to be drawn on the screen.
 * It is of type `uint16_t`, which means it is an unsigned 16-bit integer. This parameter specifies the
 * horizontal size of the rectangle in pixels.
 * @param height The `height` parameter in the `filledRectangleClip` function represents the vertical
 * dimension of the rectangle to be drawn on the display. It specifies the number of pixels from top to
 * bottom that the rectangle should cover.
 * @param colour The `colour` parameter in the `filledRectangleClip` function represents the color that
 * will be used to fill the rectangle on the display. It is of type `colour_t`, which is likely a
 * custom data type defined in your codebase to represent colors in a specific format (e.g., RGB
 * 
 * @return If the conditions in the `filledRectangleClip` function are met and the function does not
 * encounter any early return statements, then the function will not explicitly return a value. In C,
 * when a function with a return type of `void` reaches the end of its execution without encountering a
 * `return` statement, it implicitly returns without a value.
 */
static void filledRectangleClip(int16_t x, int16_t y, uint16_t width, uint16_t height, colour_t colour)
{
	int16_t x_end;
	int16_t y_end;

	if (x > (int16_t)ILI9341_LCD_WIDTH - 1 || y > (int16_t)ILI9341_LCD_HEIGHT - 1)
	{
		return;
	}

	if (x + (int16_t)width < 0 || y + (int16_t)height < 0)
	{
		return;
	}

	if (x < 0)
	{
		width -= (uint16_t)-x;
		x = 0;
	}

	if (y < 0)
	{
		height -= (uint16_t)-y;
		y = 0;
	}

	x_end = x + (int16_t)width - 1;
	if (x_end > (int16_t)ILI9341_LCD_WIDTH - 1)
	{
		x_end = (int16_t)ILI9341_LCD_WIDTH - 1;
	}

	y_end = y + (int16_t)height - 1;
	if (y_end > (int16_t)ILI9341_LCD_HEIGHT - 1)
	{
		y_end = (int16_t)ILI9341_LCD_HEIGHT - 1;
	}

	// clipped sizes
	width = (uint16_t)(x_end - x + 1);
	height = (uint16_t)(y_end - y + 1);

	ILI9341FilledRectangle((uint16_t)x, (uint16_t)y, width, height, colour);
}

/**
 ******************************************************************************
 * @file    font9.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-February-2014
 * @brief   This file provides text font9 for STM32xx-EVAL's LCD driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

static const uint8_t Font9_Table[] =
	{
		// @0 ' ' (5 pixels wide)
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @9 '!' (5 pixels wide)
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //

		// @18 '"' (5 pixels wide)
		0x50, //  # #
		0x50, //  # #
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @27 '#' (5 pixels wide)
		0x50, //  # #
		0x50, //  # #
		0xf8, // #####
		0x50, //  # #
		0xf8, // #####
		0x50, //  # #
		0x50, //  # #
		0x00, //
		0x00, //

		// @36 '$' (5 pixels wide)
		0x20, //   #
		0x78, //  ####
		0xa0, // # #
		0x70, //  ###
		0x28, //   # #
		0xf0, // ####
		0x20, //   #
		0x00, //
		0x00, //

		// @45 '%' (5 pixels wide)
		0xc0, // ##
		0xc8, // ##  #
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x98, // #  ##
		0x18, //    ##
		0x00, //
		0x00, //

		// @54 '&' (5 pixels wide)
		0x60, //  ##
		0x90, // #  #
		0xa0, // # #
		0x40, //  #
		0xa8, // # # #
		0x90, // #  #
		0x68, //  ## #
		0x00, //
		0x00, //

		// @63 ''' (5 pixels wide)
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @72 '(' (5 pixels wide)
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x40, //  #
		0x40, //  #
		0x20, //   #
		0x10, //    #
		0x00, //
		0x00, //

		// @81 ')' (5 pixels wide)
		0x40, //  #
		0x20, //   #
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x00, //
		0x00, //

		// @90 '*' (5 pixels wide)
		0x00, //
		0x20, //   #
		0xa8, // # # #
		0x70, //  ###
		0xa8, // # # #
		0x20, //   #
		0x00, //
		0x00, //
		0x00, //

		// @99 '+' (5 pixels wide)
		0x00, //
		0x20, //   #
		0x20, //   #
		0xf8, // #####
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //
		0x00, //

		// @108 ',' (5 pixels wide)
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x20, //   #
		0x40, //  #
		0x00, //

		// @117 '-' (5 pixels wide)
		0x00, //
		0x00, //
		0x00, //
		0xf8, // #####
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @126 '.' (5 pixels wide)
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //

		// @135 '/' (5 pixels wide)
		0x10, //    #
		0x10, //    #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x40, //  #
		0x40, //  #
		0x00, //
		0x00, //

		// @144 '0' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x98, // #  ##
		0xa8, // # # #
		0xc8, // ##  #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @153 '1' (5 pixels wide)
		0x20, //   #
		0x60, //  ##
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @162 '2' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x08, //     #
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0xf8, // #####
		0x00, //
		0x00, //

		// @171 '3' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x08, //     #
		0x30, //   ##
		0x08, //     #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @180 '4' (5 pixels wide)
		0x10, //    #
		0x30, //   ##
		0x50, //  # #
		0x90, // #  #
		0xf8, // #####
		0x10, //    #
		0x10, //    #
		0x00, //
		0x00, //

		// @189 '5' (5 pixels wide)
		0xf8, // #####
		0x80, // #
		0xf0, // ####
		0x08, //     #
		0x08, //     #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @198 '6' (5 pixels wide)
		0x30, //   ##
		0x40, //  #
		0x80, // #
		0xf0, // ####
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @207 '7' (5 pixels wide)
		0xf8, // #####
		0x08, //     #
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x40, //  #
		0x40, //  #
		0x00, //
		0x00, //

		// @216 '8' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @225 '9' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x78, //  ####
		0x08, //     #
		0x10, //    #
		0x60, //  ##
		0x00, //
		0x00, //

		// @234 ':' (5 pixels wide)
		0x00, //
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //
		0x00, //

		// @243 ';' (5 pixels wide)
		0x00, //
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //
		0x20, //   #
		0x40, //  #
		0x00, //
		0x00, //

		// @252 '<' (5 pixels wide)
		0x00, //
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x20, //   #
		0x10, //    #
		0x00, //
		0x00, //
		0x00, //

		// @261 '=' (5 pixels wide)
		0x00, //
		0x00, //
		0xf8, // #####
		0x00, //
		0xf8, // #####
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @270 '>' (5 pixels wide)
		0x00, //
		0x80, // #
		0x40, //  #
		0x20, //   #
		0x40, //  #
		0x80, // #
		0x00, //
		0x00, //
		0x00, //

		// @279 '?' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x08, //     #
		0x10, //    #
		0x20, //   #
		0x00, //
		0x20, //   #
		0x00, //
		0x00, //

		// @288 '@' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0xb8, // # ###
		0xb0, // # ##
		0x80, // #
		0x78, //  ####
		0x00, //
		0x00, //

		// @297 'A' (5 pixels wide)
		0x20, //   #
		0x50, //  # #
		0x88, // #   #
		0x88, // #   #
		0xf8, // #####
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @306 'B' (5 pixels wide)
		0xf0, // ####
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0x00, //
		0x00, //

		// @315 'C' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x80, // #
		0x80, // #
		0x80, // #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @324 'D' (5 pixels wide)
		0xe0, // ###
		0x90, // #  #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x90, // #  #
		0xe0, // ###
		0x00, //
		0x00, //

		// @333 'E' (5 pixels wide)
		0xf8, // #####
		0x80, // #
		0x80, // #
		0xf0, // ####
		0x80, // #
		0x80, // #
		0xf8, // #####
		0x00, //
		0x00, //

		// @342 'F' (5 pixels wide)
		0xf8, // #####
		0x80, // #
		0x80, // #
		0xf0, // ####
		0x80, // #
		0x80, // #
		0x80, // #
		0x00, //
		0x00, //

		// @351 'G' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x80, // #
		0x98, // #  ##
		0x88, // #   #
		0x88, // #   #
		0x78, //  ####
		0x00, //
		0x00, //

		// @360 'H' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0xf8, // #####
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @369 'I' (5 pixels wide)
		0x70, //  ###
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @378 'J' (5 pixels wide)
		0x38, //   ###
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x90, // #  #
		0x60, //  ##
		0x00, //
		0x00, //

		// @387 'K' (5 pixels wide)
		0x88, // #   #
		0x90, // #  #
		0xa0, // # #
		0xc0, // ##
		0xa0, // # #
		0x90, // #  #
		0x88, // #   #
		0x00, //
		0x00, //

		// @396 'L' (5 pixels wide)
		0x80, // #
		0x80, // #
		0x80, // #
		0x80, // #
		0x80, // #
		0x80, // #
		0xf8, // #####
		0x00, //
		0x00, //

		// @405 'M' (5 pixels wide)
		0x88, // #   #
		0xd8, // ## ##
		0xa8, // # # #
		0xa8, // # # #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @414 'N' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0xc8, // ##  #
		0xa8, // # # #
		0x98, // #  ##
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @423 'O' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @432 'P' (5 pixels wide)
		0xf0, // ####
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0x80, // #
		0x80, // #
		0x80, // #
		0x00, //
		0x00, //

		// @441 'Q' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0xa8, // # # #
		0x90, // #  #
		0x68, //  ## #
		0x00, //
		0x00, //

		// @450 'R' (5 pixels wide)
		0xf0, // ####
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0xa0, // # #
		0x90, // #  #
		0x88, // #   #
		0x00, //
		0x00, //

		// @459 'S' (5 pixels wide)
		0x70, //  ###
		0x88, // #   #
		0x80, // #
		0x70, //  ###
		0x08, //     #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @468 'T' (5 pixels wide)
		0xf8, // #####
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @477 'U' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @486 'V' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x50, //  # #
		0x20, //   #
		0x00, //
		0x00, //

		// @495 'W' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0xa8, // # # #
		0xa8, // # # #
		0x50, //  # #
		0x00, //
		0x00, //

		// @504 'X' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x50, //  # #
		0x20, //   #
		0x50, //  # #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @513 'Y' (5 pixels wide)
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x50, //  # #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @522 'Z' (5 pixels wide)
		0xf8, // #####
		0x08, //     #
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0x80, // #
		0xf8, // #####
		0x00, //
		0x00, //

		// @531 '[' (5 pixels wide)
		0x70, //  ###
		0x40, //  #
		0x40, //  #
		0x40, //  #
		0x40, //  #
		0x40, //  #
		0x70, //  ###
		0x00, //
		0x00, //

		// @540 '\' (5 pixels wide)
		0x40, //  #
		0x40, //  #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x10, //    #
		0x10, //    #
		0x00, //
		0x00, //

		// @549 ']' (5 pixels wide)
		0x70, //  ###
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x70, //  ###
		0x00, //
		0x00, //

		// @558 '^' (5 pixels wide)
		0x20, //   #
		0x50, //  # #
		0x88, // #   #
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @567 '_' (5 pixels wide)
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0xf8, // #####
		0x00, //
		0x00, //

		// @576 '`' (5 pixels wide)
		0x40, //  #
		0x20, //   #
		0x10, //    #
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //
		0x00, //

		// @585 'a' (5 pixels wide)
		0x00, //
		0x00, //
		0x70, //  ###
		0x08, //     #
		0x78, //  ####
		0x88, // #   #
		0x78, //  ####
		0x00, //
		0x00, //

		// @594 'b' (5 pixels wide)
		0x80, // #
		0x80, // #
		0xb0, // # ##
		0xc8, // ##  #
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0x00, //
		0x00, //

		// @603 'c' (5 pixels wide)
		0x00, //
		0x00, //
		0x70, //  ###
		0x80, // #
		0x80, // #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @612 'd' (5 pixels wide)
		0x08, //     #
		0x08, //     #
		0x68, //  ## #
		0x98, // #  ##
		0x88, // #   #
		0x88, // #   #
		0x78, //  ####
		0x00, //
		0x00, //

		// @621 'e' (5 pixels wide)
		0x00, //
		0x00, //
		0x70, //  ###
		0x88, // #   #
		0xf8, // #####
		0x80, // #
		0x70, //  ###
		0x00, //
		0x00, //

		// @630 'f' (5 pixels wide)
		0x18, //    ##
		0x20, //   #
		0x20, //   #
		0x70, //  ###
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @639 'g' (5 pixels wide)
		0x00, //
		0x00, //
		0x68, //  ## #
		0x98, // #  ##
		0x88, // #   #
		0x88, // #   #
		0x78, //  ####
		0x08, //     #
		0x70, //  ###

		// @648 'h' (5 pixels wide)
		0x80, // #
		0x80, // #
		0xb0, // # ##
		0xc8, // ##  #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @657 'i' (5 pixels wide)
		0x20, //   #
		0x00, //
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @666 'j' (5 pixels wide)
		0x00, //
		0x10, //    #
		0x00, //
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x10, //    #
		0x90, // #  #
		0x60, //  ##

		// @675 'k' (5 pixels wide)
		0x80, // #
		0x80, // #
		0x90, // #  #
		0xa0, // # #
		0xc0, // ##
		0xa0, // # #
		0x90, // #  #
		0x00, //
		0x00, //

		// @684 'l' (5 pixels wide)
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x10, //    #
		0x00, //
		0x00, //

		// @693 'm' (5 pixels wide)
		0x00, //
		0x00, //
		0xd0, // ## #
		0xa8, // # # #
		0xa8, // # # #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @702 'n' (5 pixels wide)
		0x00, //
		0x00, //
		0xb0, // # ##
		0xc8, // ##  #
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x00, //
		0x00, //

		// @711 'o' (5 pixels wide)
		0x00, //
		0x00, //
		0x70, //  ###
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x70, //  ###
		0x00, //
		0x00, //

		// @720 'p' (5 pixels wide)
		0x00, //
		0x00, //
		0xb0, // # ##
		0xc8, // ##  #
		0x88, // #   #
		0x88, // #   #
		0xf0, // ####
		0x80, // #
		0x80, // #

		// @729 'q' (5 pixels wide)
		0x00, //
		0x00, //
		0x68, //  ## #
		0x98, // #  ##
		0x88, // #   #
		0x88, // #   #
		0x78, //  ####
		0x08, //     #
		0x08, //     #

		// @738 'r' (5 pixels wide)
		0x00, //
		0x00, //
		0xb0, // # ##
		0xc8, // ##  #
		0x80, // #
		0x80, // #
		0x80, // #
		0x00, //
		0x00, //

		// @747 's' (5 pixels wide)
		0x00, //
		0x00, //
		0x70, //  ###
		0x80, // #
		0x70, //  ###
		0x08, //     #
		0xf0, // ####
		0x00, //
		0x00, //

		// @756 't' (5 pixels wide)
		0x40, //  #
		0x40, //  #
		0xe0, // ###
		0x40, //  #
		0x40, //  #
		0x48, //  #  #
		0x30, //   ##
		0x00, //
		0x00, //

		// @765 'u' (5 pixels wide)
		0x00, //
		0x00, //
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x98, // #  ##
		0x68, //  ## #
		0x00, //
		0x00, //

		// @774 'v' (5 pixels wide)
		0x00, //
		0x00, //
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x50, //  # #
		0x20, //   #
		0x00, //
		0x00, //

		// @783 'w' (5 pixels wide)
		0x00, //
		0x00, //
		0x88, // #   #
		0x88, // #   #
		0xa8, // # # #
		0xa8, // # # #
		0x50, //  # #
		0x00, //
		0x00, //

		// @792 'x' (5 pixels wide)
		0x00, //
		0x00, //
		0x88, // #   #
		0x50, //  # #
		0x20, //   #
		0x50, //  # #
		0x88, // #   #
		0x00, //
		0x00, //

		// @801 'y' (5 pixels wide)
		0x00, //
		0x00, //
		0x88, // #   #
		0x88, // #   #
		0x88, // #   #
		0x98, // #  ##
		0x68, //  ## #
		0x08, //     #
		0x70, //  ###

		// @810 'z' (5 pixels wide)
		0x00, //
		0x00, //
		0xf8, // #####
		0x10, //    #
		0x20, //   #
		0x40, //  #
		0xf8, // #####
		0x00, //
		0x00, //

		// @819 '{' (5 pixels wide)
		0x10, //    #
		0x20, //   #
		0x20, //   #
		0x40, //  #
		0x20, //   #
		0x20, //   #
		0x10, //    #
		0x00, //
		0x00, //

		// @828 '|' (5 pixels wide)
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x20, //   #
		0x00, //
		0x00, //

		// @837 '}' (5 pixels wide)
		0x40, //  #
		0x20, //   #
		0x20, //   #
		0x10, //    #
		0x20, //   #
		0x20, //   #
		0x40, //  #
		0x00, //
		0x00, //

		// @846 '~' (5 pixels wide)
		0x00, //
		0x00, //
		0x40, //  #
		0xa8, // # # #
		0x10, //    #
		0x00, //
		0x00, //
		0x00, //
		0x00, //
};

/**
 ******************************************************************************
 * @file    font16.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-February-2014
 * @brief   This file provides text font16 for STM32xx-EVAL's LCD driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

static const uint8_t Font16_Table[] =
	{
		// @0 ' ' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @32 '!' (11 pixels wide)
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @64 '"' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1D, 0xC0, //    ### ###
		0x1D, 0xC0, //    ### ###
		0x08, 0x80, //     #   #
		0x08, 0x80, //     #   #
		0x08, 0x80, //     #   #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @96 '#' (11 pixels wide)
		0x00, 0x00, //
		0x0D, 0x80, //     ## ##
		0x0D, 0x80, //     ## ##
		0x0D, 0x80, //     ## ##
		0x0D, 0x80, //     ## ##
		0x3F, 0xC0, //   ########
		0x1B, 0x00, //    ## ##
		0x3F, 0xC0, //   ########
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @128 '$' (11 pixels wide)
		0x04, 0x00, //      #
		0x1F, 0x80, //    ######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x38, 0x00, //   ###
		0x1E, 0x00, //    ####
		0x0F, 0x00, //     ####
		0x03, 0x80, //       ###
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3F, 0x00, //   ######
		0x04, 0x00, //      #
		0x04, 0x00, //      #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @160 '%' (11 pixels wide)
		0x00, 0x00, //
		0x18, 0x00, //    ##
		0x24, 0x00, //   #  #
		0x24, 0x00, //   #  #
		0x18, 0xC0, //    ##   ##
		0x07, 0x80, //      ####
		0x1E, 0x00, //    ####
		0x31, 0x80, //   ##   ##
		0x02, 0x40, //       #  #
		0x02, 0x40, //       #  #
		0x01, 0x80, //        ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @192 '&' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x0F, 0x00, //     ####
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x0C, 0x00, //     ##
		0x1D, 0x80, //    ### ##
		0x37, 0x00, //   ## ###
		0x33, 0x00, //   ##  ##
		0x1D, 0x80, //    ### ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @224 ''' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x07, 0x00, //      ###
		0x07, 0x00, //      ###
		0x02, 0x00, //       #
		0x02, 0x00, //       #
		0x02, 0x00, //       #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @256 '(' (11 pixels wide)
		0x00, 0x00, //
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x06, 0x00, //      ##
		0x0E, 0x00, //     ###
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0E, 0x00, //     ###
		0x06, 0x00, //      ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @288 ')' (11 pixels wide)
		0x00, 0x00, //
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x0C, 0x00, //     ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x1C, 0x00, //    ###
		0x18, 0x00, //    ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @320 '*' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x3F, 0xC0, //   ########
		0x3F, 0xC0, //   ########
		0x0F, 0x00, //     ####
		0x1F, 0x80, //    ######
		0x19, 0x80, //    ##  ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @352 '+' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x04, 0x00, //      #
		0x04, 0x00, //      #
		0x04, 0x00, //      #
		0x3F, 0x80, //   #######
		0x04, 0x00, //      #
		0x04, 0x00, //      #
		0x04, 0x00, //      #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @384 ',' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x04, 0x00, //      #
		0x0C, 0x00, //     ##
		0x08, 0x00, //     #
		0x08, 0x00, //     #
		0x00, 0x00, //
		0x00, 0x00, //

		// @416 '-' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x3F, 0x80, //   #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @448 '.' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @480 '/' (11 pixels wide)
		0x00, 0xC0, //         ##
		0x00, 0xC0, //         ##
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @512 '0' (11 pixels wide)
		0x00, 0x00, //
		0x0E, 0x00, //     ###
		0x1B, 0x00, //    ## ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1B, 0x00, //    ## ##
		0x0E, 0x00, //     ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @544 '1' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x3E, 0x00, //   #####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x3F, 0xC0, //   ########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @576 '2' (11 pixels wide)
		0x00, 0x00, //
		0x0F, 0x00, //     ####
		0x19, 0x80, //    ##  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x03, 0x00, //       ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x18, 0x00, //    ##
		0x30, 0x00, //   ##
		0x3F, 0x80, //   #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @608 '3' (11 pixels wide)
		0x00, 0x00, //
		0x3F, 0x00, //   ######
		0x61, 0x80, //  ##    ##
		0x01, 0x80, //        ##
		0x03, 0x00, //       ##
		0x1F, 0x00, //    #####
		0x03, 0x80, //       ###
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x61, 0x80, //  ##    ##
		0x3F, 0x00, //   ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @640 '4' (11 pixels wide)
		0x00, 0x00, //
		0x07, 0x00, //      ###
		0x07, 0x00, //      ###
		0x0F, 0x00, //     ####
		0x0B, 0x00, //     # ##
		0x1B, 0x00, //    ## ##
		0x13, 0x00, //    #  ##
		0x33, 0x00, //   ##  ##
		0x3F, 0x80, //   #######
		0x03, 0x00, //       ##
		0x0F, 0x80, //     #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @672 '5' (11 pixels wide)
		0x00, 0x00, //
		0x1F, 0x80, //    ######
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x1F, 0x00, //    #####
		0x11, 0x80, //    #   ##
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x21, 0x80, //   #    ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @704 '6' (11 pixels wide)
		0x00, 0x00, //
		0x07, 0x80, //      ####
		0x1C, 0x00, //    ###
		0x18, 0x00, //    ##
		0x30, 0x00, //   ##
		0x37, 0x00, //   ## ###
		0x39, 0x80, //   ###  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x19, 0x80, //    ##  ##
		0x0F, 0x00, //     ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @736 '7' (11 pixels wide)
		0x00, 0x00, //
		0x7F, 0x00, //  #######
		0x43, 0x00, //  #    ##
		0x03, 0x00, //       ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @768 '8' (11 pixels wide)
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @800 '9' (11 pixels wide)
		0x00, 0x00, //
		0x1E, 0x00, //    ####
		0x33, 0x00, //   ##  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x33, 0x80, //   ##  ###
		0x1D, 0x80, //    ### ##
		0x01, 0x80, //        ##
		0x03, 0x00, //       ##
		0x07, 0x00, //      ###
		0x3C, 0x00, //   ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @832 ':' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @864 ';' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x04, 0x00, //      #
		0x08, 0x00, //     #
		0x08, 0x00, //     #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @896 '<' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0xC0, //         ##
		0x03, 0x00, //       ##
		0x04, 0x00, //      #
		0x18, 0x00, //    ##
		0x60, 0x00, //  ##
		0x18, 0x00, //    ##
		0x04, 0x00, //      #
		0x03, 0x00, //       ##
		0x00, 0xC0, //         ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @928 '=' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0xC0, //  #########
		0x00, 0x00, //
		0x7F, 0xC0, //  #########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @960 '>' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x60, 0x00, //  ##
		0x18, 0x00, //    ##
		0x04, 0x00, //      #
		0x03, 0x00, //       ##
		0x00, 0xC0, //         ##
		0x03, 0x00, //       ##
		0x04, 0x00, //      #
		0x18, 0x00, //    ##
		0x60, 0x00, //  ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @992 '?' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x01, 0x80, //        ##
		0x07, 0x00, //      ###
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1024 '@' (11 pixels wide)
		0x00, 0x00, //
		0x0E, 0x00, //     ###
		0x11, 0x00, //    #   #
		0x21, 0x00, //   #    #
		0x21, 0x00, //   #    #
		0x27, 0x00, //   #  ###
		0x29, 0x00, //   # #  #
		0x29, 0x00, //   # #  #
		0x27, 0x00, //   #  ###
		0x20, 0x00, //   #
		0x11, 0x00, //    #   #
		0x0E, 0x00, //     ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1056 'A' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x3F, 0x00, //   ######
		0x0F, 0x00, //     ####
		0x09, 0x00, //     #  #
		0x19, 0x80, //    ##  ##
		0x19, 0x80, //    ##  ##
		0x1F, 0x80, //    ######
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x79, 0xE0, //  ####  ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1088 'B' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x00, //  #######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3F, 0x00, //   ######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x7F, 0x00, //  #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1120 'C' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x40, //    ##### #
		0x30, 0xC0, //   ##    ##
		0x60, 0x40, //  ##      #
		0x60, 0x00, //  ##
		0x60, 0x00, //  ##
		0x60, 0x00, //  ##
		0x60, 0x40, //  ##      #
		0x30, 0x80, //   ##    #
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1152 'D' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x00, //  #######
		0x31, 0x80, //   ##   ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x31, 0x80, //   ##   ##
		0x7F, 0x00, //  #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1184 'E' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x80, //  ########
		0x30, 0x80, //   ##    #
		0x30, 0x80, //   ##    #
		0x32, 0x00, //   ##  #
		0x3E, 0x00, //   #####
		0x32, 0x00, //   ##  #
		0x30, 0x80, //   ##    #
		0x30, 0x80, //   ##    #
		0x7F, 0x80, //  ########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1216 'F' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0xC0, //  #########
		0x30, 0x40, //   ##     #
		0x30, 0x40, //   ##     #
		0x32, 0x00, //   ##  #
		0x3E, 0x00, //   #####
		0x32, 0x00, //   ##  #
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x7C, 0x00, //  #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1248 'G' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1E, 0x80, //    #### #
		0x31, 0x80, //   ##   ##
		0x60, 0x80, //  ##     #
		0x60, 0x00, //  ##
		0x60, 0x00, //  ##
		0x67, 0xC0, //  ##  #####
		0x61, 0x80, //  ##    ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1280 'H' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3F, 0x80, //   #######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x7B, 0xC0, //  #### ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1312 'I' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x3F, 0xC0, //   ########
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x3F, 0xC0, //   ########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1344 'J' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0xC0, //    #######
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x63, 0x00, //  ##   ##
		0x63, 0x00, //  ##   ##
		0x63, 0x00, //  ##   ##
		0x3E, 0x00, //   #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1376 'K' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x33, 0x00, //   ##  ##
		0x36, 0x00, //   ## ##
		0x3C, 0x00, //   ####
		0x3E, 0x00, //   #####
		0x33, 0x00, //   ##  ##
		0x31, 0x80, //   ##   ##
		0x79, 0xC0, //  ####  ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1408 'L' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7E, 0x00, //  ######
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x40, //    ##    #
		0x18, 0x40, //    ##    #
		0x18, 0x40, //    ##    #
		0x7F, 0xC0, //  #########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1440 'M' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0xE0, 0xE0, // ###     ###
		0x60, 0xC0, //  ##     ##
		0x71, 0xC0, //  ###   ###
		0x7B, 0xC0, //  #### ####
		0x6A, 0xC0, //  ## # # ##
		0x6E, 0xC0, //  ## ### ##
		0x64, 0xC0, //  ##  #  ##
		0x60, 0xC0, //  ##     ##
		0xFB, 0xE0, // ##### #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1472 'N' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x73, 0xC0, //  ###  ####
		0x31, 0x80, //   ##   ##
		0x39, 0x80, //   ###  ##
		0x3D, 0x80, //   #### ##
		0x35, 0x80, //   ## # ##
		0x37, 0x80, //   ## ####
		0x33, 0x80, //   ##  ###
		0x31, 0x80, //   ##   ##
		0x79, 0x80, //  ####  ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1504 'O' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1536 'P' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x00, //  #######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3F, 0x00, //   ######
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x7E, 0x00, //  ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1568 'Q' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x0C, 0xC0, //     ##  ##
		0x1F, 0x80, //    ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1600 'R' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x00, //  #######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3E, 0x00, //   #####
		0x33, 0x00, //   ##  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x7C, 0xE0, //  #####  ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1632 'S' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x80, //    ######
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x38, 0x00, //   ###
		0x1F, 0x00, //    #####
		0x03, 0x80, //       ###
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x3F, 0x00, //   ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1664 'T' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x80, //  ########
		0x4C, 0x80, //  #  ##  #
		0x4C, 0x80, //  #  ##  #
		0x4C, 0x80, //  #  ##  #
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x3F, 0x00, //   ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1696 'U' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1728 'V' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x0A, 0x00, //     # #
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1760 'W' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0xFB, 0xE0, // ##### #####
		0x60, 0xC0, //  ##     ##
		0x64, 0xC0, //  ##  #  ##
		0x6E, 0xC0, //  ## ### ##
		0x6E, 0xC0, //  ## ### ##
		0x2A, 0x80, //   # # # #
		0x3B, 0x80, //   ### ###
		0x3B, 0x80, //   ### ###
		0x31, 0x80, //   ##   ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1792 'X' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x1B, 0x00, //    ## ##
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x1B, 0x00, //    ## ##
		0x31, 0x80, //   ##   ##
		0x7B, 0xC0, //  #### ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1824 'Y' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x79, 0xE0, //  ####  ####
		0x30, 0xC0, //   ##    ##
		0x19, 0x80, //    ##  ##
		0x0F, 0x00, //     ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x1F, 0x80, //    ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1856 'Z' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x3F, 0x80, //   #######
		0x21, 0x80, //   #    ##
		0x23, 0x00, //   #   ##
		0x06, 0x00, //      ##
		0x04, 0x00, //      #
		0x0C, 0x00, //     ##
		0x18, 0x80, //    ##   #
		0x30, 0x80, //   ##    #
		0x3F, 0x80, //   #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1888 '[' (11 pixels wide)
		0x00, 0x00, //
		0x07, 0x80, //      ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x07, 0x80, //      ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1920 '\' (11 pixels wide)
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x06, 0x00, //      ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x00, 0xC0, //         ##
		0x00, 0xC0, //         ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1952 ']' (11 pixels wide)
		0x00, 0x00, //
		0x1E, 0x00, //    ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x1E, 0x00, //    ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @1984 '^' (11 pixels wide)
		0x04, 0x00, //      #
		0x0A, 0x00, //     # #
		0x0A, 0x00, //     # #
		0x11, 0x00, //    #   #
		0x20, 0x80, //   #     #
		0x20, 0x80, //   #     #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2016 '_' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0xFF, 0xE0, // ###########

		// @2048 '`' (11 pixels wide)
		0x08, 0x00, //     #
		0x04, 0x00, //      #
		0x02, 0x00, //       #
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2080 'a' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x1F, 0x80, //    ######
		0x31, 0x80, //   ##   ##
		0x33, 0x80, //   ##  ###
		0x1D, 0xC0, //    ### ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2112 'b' (11 pixels wide)
		0x00, 0x00, //
		0x70, 0x00, //  ###
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x37, 0x00, //   ## ###
		0x39, 0x80, //   ###  ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x39, 0x80, //   ###  ##
		0x77, 0x00, //  ### ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2144 'c' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1E, 0x80, //    #### #
		0x31, 0x80, //   ##   ##
		0x60, 0x80, //  ##     #
		0x60, 0x00, //  ##
		0x60, 0x80, //  ##     #
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2176 'd' (11 pixels wide)
		0x00, 0x00, //
		0x03, 0x80, //       ###
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x1D, 0x80, //    ### ##
		0x33, 0x80, //   ##  ###
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x33, 0x80, //   ##  ###
		0x1D, 0xC0, //    ### ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2208 'e' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x60, 0xC0, //  ##     ##
		0x7F, 0xC0, //  #########
		0x60, 0x00, //  ##
		0x30, 0xC0, //   ##    ##
		0x1F, 0x80, //    ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2240 'f' (11 pixels wide)
		0x00, 0x00, //
		0x07, 0xE0, //      ######
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x3F, 0x80, //   #######
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x3F, 0x80, //   #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2272 'g' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1D, 0xC0, //    ### ###
		0x33, 0x80, //   ##  ###
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x33, 0x80, //   ##  ###
		0x1D, 0x80, //    ### ##
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //

		// @2304 'h' (11 pixels wide)
		0x00, 0x00, //
		0x70, 0x00, //  ###
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x37, 0x00, //   ## ###
		0x39, 0x80, //   ###  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x7B, 0xC0, //  #### ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2336 'i' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x00, 0x00, //
		0x1E, 0x00, //    ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x3F, 0xC0, //   ########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2368 'j' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x00, 0x00, //
		0x3F, 0x00, //   ######
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x03, 0x00, //       ##
		0x3E, 0x00, //   #####
		0x00, 0x00, //
		0x00, 0x00, //

		// @2400 'k' (11 pixels wide)
		0x00, 0x00, //
		0x70, 0x00, //  ###
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x37, 0x80, //   ## ####
		0x36, 0x00, //   ## ##
		0x3C, 0x00, //   ####
		0x3C, 0x00, //   ####
		0x36, 0x00, //   ## ##
		0x33, 0x00, //   ##  ##
		0x77, 0xC0, //  ### #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2432 'l' (11 pixels wide)
		0x00, 0x00, //
		0x1E, 0x00, //    ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x3F, 0xC0, //   ########
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2464 'm' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x7F, 0x80, //  ########
		0x36, 0xC0, //   ## ## ##
		0x36, 0xC0, //   ## ## ##
		0x36, 0xC0, //   ## ## ##
		0x36, 0xC0, //   ## ## ##
		0x36, 0xC0, //   ## ## ##
		0x76, 0xE0, //  ### ## ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2496 'n' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x77, 0x00, //  ### ###
		0x39, 0x80, //   ###  ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x7B, 0xC0, //  #### ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2528 'o' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x00, //    #####
		0x31, 0x80, //   ##   ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x60, 0xC0, //  ##     ##
		0x31, 0x80, //   ##   ##
		0x1F, 0x00, //    #####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2560 'p' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x77, 0x00, //  ### ###
		0x39, 0x80, //   ###  ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x30, 0xC0, //   ##    ##
		0x39, 0x80, //   ###  ##
		0x37, 0x00, //   ## ###
		0x30, 0x00, //   ##
		0x30, 0x00, //   ##
		0x7C, 0x00, //  #####
		0x00, 0x00, //
		0x00, 0x00, //

		// @2592 'q' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1D, 0xC0, //    ### ###
		0x33, 0x80, //   ##  ###
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x61, 0x80, //  ##    ##
		0x33, 0x80, //   ##  ###
		0x1D, 0x80, //    ### ##
		0x01, 0x80, //        ##
		0x01, 0x80, //        ##
		0x07, 0xC0, //      #####
		0x00, 0x00, //
		0x00, 0x00, //

		// @2624 'r' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0x80, //  #### ###
		0x1C, 0xC0, //    ###  ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x7F, 0x00, //  #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2656 's' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x1F, 0x80, //    ######
		0x31, 0x80, //   ##   ##
		0x3C, 0x00, //   ####
		0x1F, 0x00, //    #####
		0x03, 0x80, //       ###
		0x31, 0x80, //   ##   ##
		0x3F, 0x00, //   ######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2688 't' (11 pixels wide)
		0x00, 0x00, //
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x7F, 0x00, //  #######
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x00, //    ##
		0x18, 0x80, //    ##   #
		0x0F, 0x00, //     ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2720 'u' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x73, 0x80, //  ###  ###
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x33, 0x80, //   ##  ###
		0x1D, 0xC0, //    ### ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2752 'v' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x31, 0x80, //   ##   ##
		0x31, 0x80, //   ##   ##
		0x1B, 0x00, //    ## ##
		0x1B, 0x00, //    ## ##
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2784 'w' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0xF1, 0xE0, // ####   ####
		0x60, 0xC0, //  ##     ##
		0x64, 0xC0, //  ##  #  ##
		0x6E, 0xC0, //  ## ### ##
		0x3B, 0x80, //   ### ###
		0x3B, 0x80, //   ### ###
		0x31, 0x80, //   ##   ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2816 'x' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x7B, 0xC0, //  #### ####
		0x1B, 0x00, //    ## ##
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x0E, 0x00, //     ###
		0x1B, 0x00, //    ## ##
		0x7B, 0xC0, //  #### ####
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2848 'y' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x79, 0xE0, //  ####  ####
		0x30, 0xC0, //   ##    ##
		0x19, 0x80, //    ##  ##
		0x19, 0x80, //    ##  ##
		0x0B, 0x00, //     # ##
		0x0F, 0x00, //     ####
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x3E, 0x00, //   #####
		0x00, 0x00, //
		0x00, 0x00, //

		// @2880 'z' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x3F, 0x80, //   #######
		0x21, 0x80, //   #    ##
		0x03, 0x00, //       ##
		0x0E, 0x00, //     ###
		0x18, 0x00, //    ##
		0x30, 0x80, //   ##    #
		0x3F, 0x80, //   #######
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2912 '{' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x18, 0x00, //    ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x0C, 0x00, //     ##
		0x06, 0x00, //      ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2944 '|' (11 pixels wide)
		0x00, 0x00, //
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @2976 '}' (11 pixels wide)
		0x00, 0x00, //
		0x0C, 0x00, //     ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x03, 0x00, //       ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x06, 0x00, //      ##
		0x0C, 0x00, //     ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //

		// @3008 '~' (11 pixels wide)
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x18, 0x00, //    ##
		0x24, 0x80, //   #  #  #
		0x03, 0x00, //       ##
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
		0x00, 0x00, //
};
