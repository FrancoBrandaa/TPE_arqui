#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

/**
 * @brief Sets up an IDT entry with the specified index and offset.
 * 
 * @param index The index in the IDT table where the entry will be placed.
 * @param offset The memory address of the interrupt handler function.
 */
static void setup_IDT_entry(int index, uint64_t offset);

/**
 * @brief Loads and initializes the Interrupt Descriptor Table (IDT).
 * 
 * This function sets up all interrupt handlers and exception handlers
 * in the IDT, configuring the system for proper interrupt handling.
 */
void load_idt();

#endif // _IDTLOADER_H_