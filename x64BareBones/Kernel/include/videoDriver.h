#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <lib.h>
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
// Function declarations and macros for the video driver


// #define DIM_X 1024
// #define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FDS;

typedef struct {
	int x;
	int y;
}Point;

/**
 * @brief Puts a pixel on the screen at the specified coordinates with the given color.
 * 
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

/**
 * @brief Draws a filled rectangle on the screen.
 * 
 * @param topLeft Pointer to the top-left corner point of the rectangle.
 * @param downRigth Pointer to the bottom-right corner point of the rectangle.
 * @param c The color of the rectangle in hexadecimal format.
 */
void drawRectangle(Point* topLeft, Point* downRigth, uint32_t c);

/**
 * @brief Draws a character on the screen with specified foreground and background colors.
 * 
 * @param c The character to be drawn.
 * @param x The x-coordinate of the character's position.
 * @param y The y-coordinate of the character's position.
 * @param fgcolor The foreground color of the character in hexadecimal format.
 * @param bgcolor The background color of the character in hexadecimal format.
 * @param mult The size multiplier for the character.
 */
void drawChar(unsigned char c, int x, int y, int fgcolor, int bgcolor, int mult);


void vd_putChar(char c, FDS fd);

/**
 * @brief Draws a circle on the screen.
 * 
 * @param centerX The x-coordinate of the circle's center.
 * @param centerY The y-coordinate of the circle's center.
 * @param radius The radius of the circle.
 * @param color The color of the circle in hexadecimal format.
 */
void drawCircle(int centerX, int centerY, int radius, uint32_t color);

void vd_setCursor(int x, int y);
void getCursorPos(int* x, int* y);
void vd_cleanScreen(void);
void vd_printstr(FDS fd, const char * buf, size_t count);
void changeBackgroundColor(uint32_t hexColor);
void vd_setZoom(int new_zoom);
void setFontColor(uint32_t hexColor);
void scroll_screen(void);


#endif // VIDEODRIVER_H