/***************************************************
  libc.h
****************************************************/


#ifndef _LIBC_H_
#define _LIBC_H_
#include <stdint.h>
#include <stddef.h>
#include <libasm.h>


#define zoom_user 1
#define ZOOM_MAX 5
#define ZOOM_MIN 1

struct vbeInfo {
    uint16_t width;        // width in pixels
    uint16_t height;  
};
typedef struct vbeInfo * neededInfo;

#define CHAR_WIDTH (BASE_CHAR_WIDTH*zoom_user)       // Ancho de un char actual [EN PIXELES]
#define CHAR_HEIGHT (BASE_CHAR_HEIGHT*zoom_user)     // Alto de un char actual [EN PIXELES]

#define DIM_CHAR_Y (DIM_Y/CHAR_HEIGHT)      // Ancho de pantalla [EN CHARS ACTUALES]
#define DIM_CHAR_X (DIM_X/CHAR_WIDTH)       // Alto de pantalla [EN CHARS ACTUALES]
#define COMMAND_LINE_X (2*BASE_CHAR_WIDTH)            // Pos de x de la linea de comandos [EN PIXELES]
#define COMMAND_LINE_Y (DIM_Y-(2*CHAR_HEIGHT))        // Pos de y de la linea de comandos [EN PIXELES]
#define COMMAND_LINE_Y_exec (DIM_Y-(20*CHAR_HEIGHT))        // Pos de y del comando en la shell [EN PIXELES]
#define COMMAND_DIM ((BASE_DIM_CHAR_X - 4) * 2)

extern uint16_t DIM_X;// Ancho de pantalla [EN PIXELES]
extern uint16_t DIM_Y; // Alto de pantalla [EN PIXELES]        

#define BASE_CHAR_WIDTH 8           // Ancho de un char base [EN PIXELES]
#define BASE_CHAR_HEIGHT 16         // Ancho de un char base [EN PIXELES]

#define BASE_DIM_CHAR_Y (DIM_Y/BASE_CHAR_HEIGHT)      // Ancho de pantalla [EN CHARS BASE]
#define BASE_DIM_CHAR_X (DIM_X/BASE_CHAR_WIDTH)       // Alto de pantalla [EN CHARS BASE]

#define NOT_KEY -2 //pidir al kernel
#define KEY_ESC 27  //pedir al kernel

typedef struct {
	int x;
	int y;
}Point;


typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FDS;

typedef struct dateStruct {
    char sec;
    char min;
    char hour;
    char day;
    char month;
    char year;
} dateStruct;


int initScreenSize(void);
int getVbeInfo(neededInfo info);
/**
 * @brief Puts a pixel on the screen at the specified coordinates with the given color.
 * 
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t color,uint64_t x, uint64_t y);

void sleep(uint64_t seconds);

void playTone(uint64_t frequency, uint64_t duration_ms);

void drawCircle(int centerX, int centerY, int radius, uint32_t color);

dateStruct * getDate ();
/**
 * @brief Retrieves the number of ticks since the system started.
 * 
 * @return uint64_t Number of ticks.
 */
uint64_t getTicks();
/**
 * @brief Draws a rectangle on the screen.
 * 
 * @param topLeft Pointer to a Point structure representing the top-left corner of the rectangle.
 * @param downRigth Pointer to a Point structure representing the bottom-right corner of the rectangle.
 * @param color The color of the rectangle in hexadecimal format.
 * 
 * @return void
 */

void drawRectangle(Point topLeft, Point downRigth, uint32_t color);
/**
 * @brief Displays the current saved values of the CPU registers.
 */
void showRegisters();

/**
 * @brief Retrieves the current system time.
 * 
 * @return time* Pointer to a time structure containing the current time.
 */
dateStruct * getDate();

/**
 * @brief Sets the background color of the display.
 * 
 * @param hexColor The color to set as the background, in hexadecimal format.
 */
void setBackGroundColor(uint32_t hexColor);


/**
 * @brief Sets the font color for text display.
 * 
 * @param hexColor The color to set for the font, in hexadecimal format.
 */
void setFontColor(uint32_t hexColor);

/**
 * @brief Prints a string to the display.
 * 
 * @param buf The string to print.
 */
void print(const char * buf);

/**
 * @brief Prints a string to the display with a specified length.
 * 
 * @param buf The string to print.
 * @param lenght The length of the string.
 */
void nprint(const char * buf, uint64_t lenght);



/**
 * @brief Prints an integer value to the display.
 *
 * @param value The integer value to print.
 */
void printInt(int value);

/**
 * @brief Reads input from the user into a buffer.
 * 
 * @param buf The buffer to store the input.
 * @param count The maximum number of characters to read.
 * @return int The number of characters read.
 */
int scan(char * buf, uint32_t count);

/**
 * @brief Converts an integer to a string.
 * 
 * @param value The integer value to convert.
 * @param buffer The buffer to store the resulting string.
 * @param base The numerical base for the conversion.
 * @param n The minimum number of digits to output.
 * @return int The length of the resulting string.
 */
int itoa(uint64_t value, char * buffer, int base, int n);

/**
 * @brief Waits for and returns a character input from the user.
 * 
 * @return char The character input by the user.
 */
char getChar();

/**
 * @brief Outputs a character to the display.
 * 
 * @param c The character to output.
 */
void putChar(char c);

/**
 * @brief Calculates the length of a string.
 * 
 * @param str The string to measure.
 * @return int The length of the string.
 */
int strlen(const char * str);

/**
 * @brief Sets the cursor position on the display.
 * 
 * @param x The x-coordinate of the cursor.
 * @param y The y-coordinate of the cursor.
 */
void setCursor(uint32_t x, uint32_t y);

/**
 * @brief Sets the character cursor position on the display.
 * 
 * @param x The x-coordinate of the character cursor.
 * @param y The y-coordinate of the character cursor.
 */
void setCharCursor(uint32_t x, uint32_t y);

/**
 * @brief Converts a string to an integer.
 * 
 * @param str The string to convert.
 * @return int The converted integer value.
 */
int atoi(const char *str);


int strcmp(const char* s1, const char* s2);

void setZoom(int new_zoom);

int strncmp(const char* s1, const char* s2, size_t n);

void swapBuffers(void);

/**
 * @brief Copies up to n characters from source to destination.
 * 
 * @param source The source string.
 * @param dest The destination buffer.
 * @param n The maximum number of characters to copy.
 * @return char* The destination buffer.
 */
char* strNCpy(const char * source, char * dest, int n);

enum colors{
    black = 0x000000,
    white = 0xFFFFFF,
    blue = 0x0000FF,
    green = 0x00FF00,
    red = 0xFF0000,
    yellow = 0xFFFF00,
    purple = 0xFF00FF,
    cyan = 0x00FFFF,
    orange = 0xFFA500,
    pink = 0xFFC0CB,
    brown = 0xA52A2A,
    lightGrey = 0xD3D3D3,
    darkGrey = 0x555555, 
    lightBlue = 0xADD8E6,
    lightGreen = 0x90EE90,
    lightRed = 0xFFA07A,
    lightPink = 0xFFB6C1,
    lightBrown = 0xD2B48C,
    darkBlue = 0x00008B,
    darkGreen = 0x006400,
    darkRed = 0x8B0000,
    darkYellow = 0x9B870C,
    darkPurple = 0x800080,
    darkCyan = 0x008B8B,
    darkOrange = 0xFF8C00,
    darkPink = 0xFF1493,
    darkBrown = 0x654321
};

#endif
