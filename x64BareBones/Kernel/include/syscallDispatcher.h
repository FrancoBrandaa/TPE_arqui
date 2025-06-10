#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>
#include <lib.h>
#include <stdarg.h>
#include <interrupts.h>
#include <time.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <soundDriver.h>

/**
 * @brief Main system call dispatcher function.
 * 
 * @param rax System call number identifying which service to execute.
 * @param ... Variable arguments passed from user-space for the system call.
 * @return uint64_t Return value from the executed system call.
 */
uint64_t sysCallDispatcher(uint64_t rax, ...);

/**
 * @brief Sets the font color for text rendering.
 * 
 * @param hexColor The color in hexadecimal format (0xRRGGBB).
 */
void setFontColor(uint32_t hexColor);

/**
 * @brief Writes data to a file descriptor (typically STDOUT or STDERR).
 * 
 * @param fd File descriptor to write to (STDOUT, STDERR, etc.).
 * @param buf Buffer containing the data to write.
 * @param count Number of bytes to write from the buffer.
 */
void sysWrite(FDS fd, const char *buf, size_t count);

/**
 * @brief Reads characters from the keyboard buffer.
 * 
 * @param buf Buffer to store the read characters.
 * @param count Maximum number of characters to read.
 * @return uint32_t Number of characters actually read.
 */
uint32_t readChars(char *buf, size_t count);

/**
 * @brief Reads data from a file descriptor (typically STDIN).
 * 
 * @param fd File descriptor to read from (STDIN, etc.).
 * @param buf Buffer to store the read data.
 * @param count Maximum number of bytes to read.
 * @return size_t Number of bytes actually read.
 */
size_t sysRead(FDS fd, char *buf, size_t count);

/**
 * @brief Sets the text zoom factor for rendering.
 * 
 * @param new_zoom New zoom factor (typically 1-5).
 */
void setZoom(int new_zoom);

/**
 * @brief Swaps the front and back video buffers for double buffering.
 */
void swapBuffers(void);

#endif