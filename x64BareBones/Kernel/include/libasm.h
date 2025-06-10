/**
 * @file libasm.h
 * @brief Header file for assembly-related functions and system calls.
 *
 * This file contains declarations for functions that interact with hardware
 * and perform low-level operations such as port I/O and CPU vendor identification.
 */

void _hlt(void);

void _sti(void);
void _cli(void);
/**
 * @brief Outputs a byte to the specified port.
 *
 * @param port The I/O port to write to.
 * @param value The byte value to write to the port.
 */
void outb(unsigned short port, unsigned char value);

/**
 * @brief Inputs a byte from the specified port.
 *
 * @param port The I/O port to read from.
 * @return The byte value read from the port.
 */
unsigned char inb(unsigned short port);

