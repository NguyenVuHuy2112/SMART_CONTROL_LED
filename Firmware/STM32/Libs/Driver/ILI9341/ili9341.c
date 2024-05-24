/*********************
 *      INCLUDES
 *********************/

#include "ili9341.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "delay_timer.h"
/**********************
 *  EXTERN VARIABLES
 **********************/

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;

/**********************
 *  STATIC VARIABLES
 **********************/

static volatile bool txComplete;

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static uint8_t SPI_Master_write_color(uint16_t color, uint16_t size);
static void SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
static void WriteCommand(uint8_t command);
static void WriteData(uint8_t data);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The ILI9341Reset function resets the ILI9341 display by toggling the reset pin.
 */
void ILI9341Reset(void)
{
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
	DELAY_Tim_Ms(&htim1, 200UL);
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
	DELAY_Tim_Ms(&htim1, 200UL);
}

/**
 * The ILI9341Init function initializes an ILI9341 display module with a series of commands and data
 * writes.
 */
void ILI9341Init(void)
{
	HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);

	WriteCommand(0x01U);
	DELAY_Tim_Ms(&htim1, 1000UL);
	WriteCommand(0xCBU);
	WriteData(0x39U);
	WriteData(0x2CU);
	WriteData(0x00U);
	WriteData(0x34U);
	WriteData(0x02U);
	WriteCommand(0xCFU);
	WriteData(0x00U);
	WriteData(0xC1U);
	WriteData(0x30U);
	WriteCommand(0xE8U);
	WriteData(0x85U);
	WriteData(0x00U);
	WriteData(0x78U);
	WriteCommand(0xEAU);
	WriteData(0x00U);
	WriteData(0x00U);
	WriteCommand(0xEDU);
	WriteData(0x64U);
	WriteData(0x03U);
	WriteData(0x12U);
	WriteData(0x81U);
	WriteCommand(0xF7U);
	WriteData(0x20U);
	WriteCommand(0xC0U);
	WriteData(0x23U);
	WriteCommand(0xC1U);
	WriteData(0x10U);
	WriteCommand(0xC5U);
	WriteData(0x3EU);
	WriteData(0x28U);
	WriteCommand(0xC7U);
	WriteData(0x86U);
	WriteCommand(0x36U);
	WriteData(0x48U);
	WriteCommand(0x3AU);
	WriteData(0x55U);
	WriteCommand(0xB1U);
	WriteData(0x00U);
	WriteData(0x18U);
	WriteCommand(0xB6U);
	WriteData(0x08U);
	WriteData(0x82U);
	WriteData(0x27U);
	WriteCommand(0xF2U);
	WriteData(0x00U);
	WriteCommand(0x26U);
	WriteData(0x01U);
	WriteCommand(0xE0U);
	WriteData(0x0FU);
	WriteData(0x31U);
	WriteData(0x2BU);
	WriteData(0x0CU);
	WriteData(0x0EU);
	WriteData(0x08U);
	WriteData(0x4EU);
	WriteData(0xF1U);
	WriteData(0x37U);
	WriteData(0x07U);
	WriteData(0x10U);
	WriteData(0x03U);
	WriteData(0x0EU);
	WriteData(0x09U);
	WriteData(0x00U);
	WriteCommand(0xE1U);
	WriteData(0x00U);
	WriteData(0x0EU);
	WriteData(0x14U);
	WriteData(0x03U);
	WriteData(0x11U);
	WriteData(0x07U);
	WriteData(0x31U);
	WriteData(0xC1U);
	WriteData(0x48U);
	WriteData(0x08U);
	WriteData(0x0FU);
	WriteData(0x0CU);
	WriteData(0x31U);
	WriteData(0x36U);
	WriteData(0x0FU);
	WriteCommand(0x11U);
	DELAY_Tim_Ms(&htim1, 120UL);
	WriteCommand(0x29U);
	WriteCommand(0x36U);
	WriteData(0x48U);
}

/**
 * The ILI9341Pixel function sets a pixel at a specified position on an ILI9341 LCD display with a
 * specified color.
 * 
 * @param x The x parameter represents the x-coordinate of the pixel on the ILI9341 LCD display where
 * you want to set the color.
 * @param y The `y` parameter represents the vertical coordinate of the pixel on the ILI9341 LCD
 * display where you want to set the color specified by the `colour` parameter.
 * @param colour The `colour` parameter in the `ILI9341Pixel` function is of type `colour_t`, which is
 * a user-defined type representing a color value. It is being passed to the function to specify the
 * color of the pixel to be drawn at the coordinates `(x, y)` on the IL
 * 
 * @return If the condition `x >= ILI9341_LCD_WIDTH || y >= ILI9341_LCD_HEIGHT` is true, the function
 * will return without performing any further actions.
 */
void ILI9341Pixel(uint16_t x, uint16_t y, colour_t colour)
{
	colour_t beColour = __builtin_bswap16(colour);

	if (x >= ILI9341_LCD_WIDTH || y >= ILI9341_LCD_HEIGHT)
	{
		return;
	}

	SetWindow(x, y, x, y);

	HAL_SPI_Transmit(&hspi1, (uint8_t *)&beColour, 2U, 100UL);
}

/**
 * The function `ILI9341DrawColourBitmap` draws a bitmap image on an ILI9341 display at a specified
 * position with the given width and height using SPI communication.
 * 
 * @param x The x-coordinate of the top-left corner where the bitmap will be drawn.
 * @param y The `y` parameter represents the vertical position where the top-left corner of the bitmap
 * will be drawn on the display.
 * @param width The `width` parameter in the `ILI9341DrawColourBitmap` function represents the width of
 * the bitmap image that you want to draw on the ILI9341 display. It specifies the number of pixels in
 * the horizontal direction of the image.
 * @param height The `height` parameter in the `ILI9341DrawColourBitmap` function represents the height
 * of the bitmap image that you want to draw on the ILI9341 display. It specifies the number of rows of
 * pixels in the image.
 * @param imageData The `imageData` parameter is a pointer to an array of uint8_t data representing the
 * pixel colors of the bitmap image. Each pair of consecutive uint8_t values in the array represents a
 * single pixel color in RGB565 format (16-bit color). The first uint8_t represents the lower
 */
void ILI9341DrawColourBitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *imageData) {

    SetWindow(x, y, x + width - 1U, y + height - 1U);

    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);

    for (uint32_t i = 0; i < width * height * 2; i += 2) {
        uint16_t color = (imageData[i + 1] << 8) | imageData[i];

        HAL_SPI_Transmit(&hspi1, (uint8_t*)&color, sizeof(color), 200U);
    }
}

/**
 * The ILI9341FilledRectangle function fills a rectangle on an ILI9341 display with a specified color.
 * 
 * @param x The x-coordinate of the top-left corner of the filled rectangle.
 * @param y The `y` parameter represents the vertical position of the top-left corner of the filled
 * rectangle on the display.
 * @param width The width of the filled rectangle.
 * @param height The height parameter in the ILI9341FilledRectangle function represents the vertical
 * size of the filled rectangle to be drawn on the display.
 * @param colour The `colour` parameter represents the color that will be used to fill the rectangle.
 * It is of type `colour_t`, which is likely a custom data type defined elsewhere in your codebase to
 * represent colors in a specific format (e.g., RGB565).
 */
void ILI9341FilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, colour_t colour)
{
	SetWindow(x, y, x + width - 1U, y + height - 1U);
    for(int i = x; i < x + width -1; i++)
    {
        uint16_t size = height + 2;
        SPI_Master_write_color(colour, size);
    }
}

/********************
 *  STATIC FUNCTION
 ********************/

/**
 * The WriteCommand function writes a command to a specific GPIO pin using SPI communication.
 * 
 * @param command The `command` parameter is a uint8_t type variable, which is an unsigned 8-bit
 * integer used to specify a command to be sent to a display controller.
 */
static void WriteCommand(uint8_t command)
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &command, 1U, 100U);
}

/**
 * The WriteData function writes a single byte of data using SPI communication with a specific GPIO pin
 * set high.
 * 
 * @param data The `data` parameter is of type `uint8_t`, which means it is an unsigned 8-bit integer
 * representing a single byte of data to be transmitted.
 */
static void WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspi1, &data, 1U, 200U);
}

/**
 * The SetWindow function sets the display window coordinates for a specific area on the screen.
 * 
 * @param xStart The starting x-coordinate of the window.
 * @param yStart The `yStart` parameter represents the starting y-coordinate of the window that you
 * want to set on the display.
 * @param xEnd The `xEnd` parameter represents the ending x-coordinate of the window that you want to
 * set on the display.
 * @param yEnd The `yEnd` parameter represents the ending y-coordinate of the window that is being set
 * on the display.
 */
static void SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
	WriteCommand(0x2AU);
	WriteData(xStart >> 8);
	WriteData(xStart);
	WriteData(xEnd >> 8);
	WriteData(xEnd);

	WriteCommand(0x2BU);
	WriteData(yStart >> 8);
	WriteData(yStart);
	WriteData(yEnd >> 8);
	WriteData(yEnd);

	WriteCommand(0x2CU);

	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
}

/**
 * The function `SPI_Master_write_color` writes a specified color to a SPI device with a given size.
 * 
 * @param color The `color` parameter is a 16-bit value representing the color to be written. It is
 * split into two bytes, where the most significant byte is shifted right by 8 bits and the least
 * significant byte is obtained by masking with 0xFF.
 * @param size The `size` parameter in the `SPI_Master_write_color` function represents the number of
 * times the specified color value should be written to the `Byte` array. Each color value consists of
 * 2 bytes (16 bits), so the total number of bytes written to the `Byte` array will be
 * 
 * @return The function `SPI_Master_write_color` is returning the result of the `HAL_SPI_Transmit`
 * function, which is the status of the SPI transmission operation.
 */
static uint8_t SPI_Master_write_color(uint16_t color, uint16_t size)
{
	static uint8_t Byte[1024];
	uint16_t index = 0;
	for(uint16_t i = 0; i < size; i++)
    {
		Byte[index++] = (color >> 8) & 0xFF;
		Byte[index++] = color & 0xFF;
	}
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
	return HAL_SPI_Transmit(&hspi1, Byte, size*2, 200U);
}
