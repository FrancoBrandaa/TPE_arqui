#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>

//==============================================================================
// HARDWARE INTERRUPT HANDLERS (IRQ)
//==============================================================================

/**
 * @brief Handler for IRQ0 - System Timer Tick.
 * 
 * This interrupt occurs approximately every 55ms and is used for 
 * system timing and scheduling.
 */
void _irq00Handler(void);

/**
 * @brief Handler for IRQ1 - Keyboard interrupt.
 * 
 * This interrupt is triggered when a key is pressed or released
 * on the keyboard.
 */
void _irq01Handler(void);

/**
 * @brief Handler for IRQ2 - Cascade signal from master PIC.
 * 
 * This IRQ is used internally by the PIC controllers for cascading
 * and should not be used by external devices.
 */
void _irq02Handler(void);

/**
 * @brief Handler for IRQ3 - Serial ports 2 and 4.
 * 
 * This interrupt handles communication through COM2 and COM4 ports.
 */
void _irq03Handler(void);

/**
 * @brief Handler for IRQ4 - Serial ports 1 and 3.
 * 
 * This interrupt handles communication through COM1 and COM3 ports.
 */
void _irq04Handler(void);

/**
 * @brief Handler for IRQ5 - Miscellaneous devices (USB, sound cards, etc.).
 * 
 * The specific device depends on the hardware configuration.
 */
void _irq05Handler(void);

//==============================================================================
// EXCEPTION HANDLERS
//==============================================================================

/**
 * @brief Handler for exception 0 - Division by zero.
 * 
 * This exception is triggered when attempting to divide by zero.
 */
void _exception0Handler(void);

/**
 * @brief Handler for exception 6 - Invalid opcode.
 * 
 * This exception is triggered when the CPU encounters an invalid
 * or unrecognized instruction.
 */
void _exception6Handler(void);

//==============================================================================
// SOFTWARE INTERRUPT HANDLER
//==============================================================================

/**
 * @brief Handler for int 0x80 - System calls.
 * 
 * This interrupt provides the interface for user-space programs
 * to request kernel services.
 */
void _int80Handler(void);

//==============================================================================
// AUXILIARY FUNCTIONS
//==============================================================================

/**
 * @brief CLI instruction: disables interrupts at CPU level.
 */
void _cli(void);

/**
 * @brief STI instruction: enables interrupts at CPU level.
 */
void _sti(void);

/**
 * @brief HLT instruction: halts the CPU until the next interrupt.
 */
void _hlt(void);

/**
 * @brief Sets the master PIC mask.
 * 
 * @param mask Defines which IRQs are enabled/disabled on the master PIC (0x21).
 */
void picMasterMask(uint8_t mask);

/**
 * @brief Sets the slave PIC mask.
 * 
 * @param mask Defines which IRQs are enabled/disabled on the slave PIC (0xA1).
 */
void picSlaveMask(uint8_t mask);

//==============================================================================
// DEBUG AND REGISTER FUNCTIONS
//==============================================================================

/**
 * @brief Saves the current CPU register state.
 * 
 * This function is typically called before handling exceptions
 * to preserve the register context.
 */
void saveRegisters(void);

/**
 * @brief Returns a pointer to the saved CPU registers.
 * 
 * @return uint64_t* Pointer to the register backup if saved; NULL if not saved.
 */
uint64_t * getRegisters(void);

/**
 * @brief Halts CPU execution (combines CLI + HLT).
 * 
 * This function disables interrupts and halts the CPU completely.
 */
void haltcpu(void);

#endif /* INTERRUPS_H_ */