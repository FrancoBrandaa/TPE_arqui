#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <lib.h>
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H


struct vbeInfo {
    uint16_t width;        // width in pixels
    uint16_t height;  
};
typedef struct vbeInfo * neededInfo;

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

void putChar(char c, FDS fd);
void setCursor(int x, int y);
void cleanScreen(void);
void printStr(FDS fd, const char * buf, size_t count);
void changeBackgroundColor(uint32_t hexColor);
void setZoom(int new_zoom);
void setFontColor(uint32_t hexColor);
void scrollScreen(void);
void setCursor(int x, int y);
void setFontColor(uint32_t newColor);
void SetBackgroundColor(uint32_t newColor);
int getVbeInfo(neededInfo info);
void replaceColor(uint32_t oldColor, uint32_t newColor);

// Double Buffering Functions
void swapBuffers(void);
void clearBackBuffer(uint32_t hexColor);


extern uint8_t font_bitmap[256 * CHAR_HEIGHT];


#endif // VIDEODRIVER_H