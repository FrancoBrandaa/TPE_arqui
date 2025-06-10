#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <lib.h>

//==============================================================================
// STRUCTURES AND TYPES
//==============================================================================

/**
 * @brief Structure containing VBE (Video BIOS Extension) information.
 */
struct vbeInfo {
    uint16_t width;     /**< Screen width in pixels */
    uint16_t height;    /**< Screen height in pixels */
};
typedef struct vbeInfo * neededInfo;

/**
 * @brief Standard character dimensions in pixels.
 */
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

/**
 * @brief File descriptor enumeration for standard I/O streams.
 */
typedef enum{
    STDIN = 0,   /**< Standard input */
    STDOUT,      /**< Standard output */
    STDERR,      /**< Standard error */
} FDS;

/**
 * @brief Point structure for 2D coordinates.
 */
typedef struct {
    int x;  /**< X coordinate */
    int y;  /**< Y coordinate */
} Point;

//==============================================================================
// FUNCTION DECLARATIONS
//==============================================================================

/**
 * @brief Places a pixel on the screen at specified coordinates with given color.
 * 
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

/**
 * @brief Draws a character at the current cursor position.
 * 
 * @param c The character to draw.
 * @param color The foreground color of the character.
 * @param bgColor The background color of the character.
 */
void drawChar(char c, uint32_t color, uint32_t bgColor);

/**
 * @brief Prints a string to the screen at the current cursor position.
 * 
 * @param fd File descriptor (STDOUT or STDERR).
 * @param str The string to print.
 * @param count The number of characters to print.
 */
void printStr(FDS fd, const char *str, uint32_t count);

/**
 * @brief Sets the cursor position in pixel coordinates.
 * 
 * @param x The x-coordinate in pixels.
 * @param y The y-coordinate in pixels.
 */
void setCursor(uint32_t x, uint32_t y);

/**
 * @brief Gets the current cursor X position.
 * 
 * @return uint32_t Current X position in pixels.
 */
uint32_t getCursorX();

/**
 * @brief Gets the current cursor Y position.
 * 
 * @return uint32_t Current Y position in pixels.
 */
uint32_t getCursorY();

/**
 * @brief Clears the entire screen with the background color.
 */
void cleanScreen();

/**
 * @brief Sets the font color for text rendering.
 * 
 * @param hexColor The color in hexadecimal format.
 */
void setFontColor(uint32_t hexColor);

/**
 * @brief Sets the background color for text rendering.
 * 
 * @param hexColor The background color in hexadecimal format.
 */
void setBackGroundColor(uint32_t hexColor);

/**
 * @brief Sets the zoom factor for text rendering.
 * 
 * @param newZoom The new zoom factor (1-5).
 */
void setZoom(uint8_t newZoom);

/**
 * @brief Gets VBE information including screen dimensions.
 * 
 * @param info Pointer to structure to fill with VBE information.
 * @return int 0 on success, non-zero on error.
 */
int getVbeInfo(neededInfo info);

/**
 * @brief Swaps the front and back buffers for double buffering.
 */
void swapBuffers();

/**
 * @brief Draws a filled circle on the screen.
 * 
 * @param centerX X coordinate of the circle center.
 * @param centerY Y coordinate of the circle center.
 * @param radius Radius of the circle in pixels.
 * @param color Color of the circle in hexadecimal format.
 */
void drawCircle(int centerX, int centerY, int radius, uint32_t color);

/**
 * @brief Draws a filled rectangle on the screen.
 * 
 * @param topLeft Top-left corner coordinates.
 * @param bottomRight Bottom-right corner coordinates.
 * @param color Color of the rectangle in hexadecimal format.
 */
void drawRectangle(Point topLeft, Point bottomRight, uint32_t color);

#endif // VIDEODRIVER_H