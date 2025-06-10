/**
 * @file libasm.h
 * @brief Header file for assembly-related functions and system calls.
 *
 * This file contains declarations for functions that interact with hardware
 * and perform low-level operations such as port I/O and CPU control.
 */

/**
 * @brief Halts the CPU until the next interrupt.
 * 
 * Executes the HLT instruction, putting the processor in a low-power
 * state until an interrupt occurs.
 */
void _hlt(void);

/**
 * @brief Enables interrupts at the CPU level.
 * 
 * Executes the STI instruction, allowing the CPU to respond to
 * maskable interrupts.
 */
void _sti(void);

/**
 * @brief Disables interrupts at the CPU level.
 * 
 * Executes the CLI instruction, preventing the CPU from responding
 * to maskable interrupts.
 */
void _cli(void);

/**
 * @brief Outputs a byte to the specified I/O port.
 *
 * @param port The I/O port address to write to.
 * @param value The byte value to write to the port.
 */
void outb(unsigned short port, unsigned char value);

/**
 * @brief Inputs a byte from the specified I/O port.
 *
 * @param port The I/O port address to read from.
 * @return The byte value read from the port.
 */
unsigned char inb(unsigned short port);