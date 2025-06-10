#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <videoDriver.h>
#include <interrupts.h>

/**
 * @brief Copies up to n characters from source to destination string.
 * 
 * @param src The source string to copy from.
 * @param dest The destination buffer to copy to.
 * @param n The maximum number of characters to copy.
 * @return char* Pointer to the destination string.
 */
char* strNCpy(const char *src, char *dest, int n);

/**
 * @brief Sets a block of memory to a specific value.
 * 
 * @param destination Pointer to the memory block to fill.
 * @param character The value to set (converted to unsigned char).
 * @param length The number of bytes to set.
 * @return void* Pointer to the destination memory block.
 */
void * memset(void * destination, int32_t character, uint64_t length);

/**
 * @brief Copies memory from source to destination.
 * 
 * @param destination Pointer to the destination memory block.
 * @param source Pointer to the source memory block.
 * @param length The number of bytes to copy.
 * @return void* Pointer to the destination memory block.
 */
void * memcpy(void * destination, const void * source, uint64_t length);

/**
 * @brief Displays the current CPU register values on screen.
 * 
 * This function shows all general-purpose registers, instruction pointer,
 * and flags in a formatted display for debugging purposes.
 */
void showRegisters();

/**
 * @brief Calculates the length of a null-terminated string.
 * 
 * @param str The string to measure.
 * @return int The length of the string (excluding null terminator).
 */
int strLen(char * str);

#endif